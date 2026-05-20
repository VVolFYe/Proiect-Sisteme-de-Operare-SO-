#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t keep_alive = 1;

static void handle_monitor_line(const char *line, int *ended) {
    if (strncmp(line, "ERROR:", 6) == 0) {
        printf("MONITOR ERROR:%s\n", line + 6);
        *ended = 1;
    } else if (strncmp(line, "DONE:", 5) == 0) {
        printf("MONITOR DONE:%s\n", line + 5);
        *ended = 1;
    } else if (strncmp(line, "INFO:", 5) == 0) {
        printf("MONITOR INFO:%s\n", line + 5);
    } else {
        printf("MONITOR MESSAGE:%s\n", line);
    }
    fflush(stdout);
}

void start_monitor() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(-1);
    }

    pid_t hub_mon = fork();
    if (hub_mon < 0) {
        perror("fork");
        exit(-1);
    }

    if (hub_mon == 0) {
        pid_t monitor_pid = fork();
        if (monitor_pid < 0) {
            perror("fork");
            exit(-1);
        }

        if (monitor_pid == 0) {
            close(pipefd[0]);
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(-1);
            }
            close(pipefd[1]);
            execlp("./monitor_reports", "./monitor_reports", NULL);
            perror("execlp");
            exit(-1);
        }

        close(pipefd[1]);

        char buf[1024];
        char line[2048];
        size_t line_len = 0;
        int monitor_ended = 0;
        ssize_t r;

        while ((r = read(pipefd[0], buf, sizeof(buf))) > 0) {
            for (ssize_t i = 0; i < r; i++) {
                if (buf[i] == '\n') {
                    line[line_len] = '\0';
                    handle_monitor_line(line, &monitor_ended);
                    line_len = 0;
                } else if (line_len + 1 < sizeof(line)) {
                    line[line_len++] = buf[i];
                }
            }
        }

        if (line_len > 0) {
            line[line_len] = '\0';
            handle_monitor_line(line, &monitor_ended);
        }

        close(pipefd[0]);
        waitpid(monitor_pid, NULL, 0);

        if (!monitor_ended) {
            printf("MONITOR DONE: monitor process ended.\n");
            fflush(stdout);
        }

        exit(EXIT_SUCCESS);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    signal(SIGCHLD, SIG_IGN);
}

int main() {
    start_monitor();

    while (keep_alive) {
        sleep(1);
    }

    return 0;
}