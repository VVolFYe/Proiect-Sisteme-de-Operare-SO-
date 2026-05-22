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
    // signal(SIGCHLD, SIG_IGN); //not needed anymore
}

//spargem linia in argumente
int split_command(char *line, char *args[]) {
    int count = 0;

    char *p = strtok(line, " \n\t");

    while (p != NULL && count < 31) {
        args[count] = p;
        count++;
        p = strtok(NULL, " \n\t");
    }

    args[count] = NULL; //punem ultimul pe null

    return count;
}

void calculate_scores_command(int count, char *districts[]) {
    if (count == 0) {
        printf("Calculate scores calculates commands for the districts received in arguments. There are not any.\n\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        int pipefd[2];

        if (pipe(pipefd) == -1) {
            perror("pipe");
            continue;
        }

        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            continue;
        }

        if (pid == 0) {
            close(pipefd[0]);

            if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                perror("dup2");
                exit(-1);
            }

            close(pipefd[1]);

            execlp("./calculate_scores", "./calculate_scores", districts[i], NULL);

            fprintf(stderr, "execlp error.\n");
            exit(-1);
        }

        close(pipefd[1]);

        printf("\n[district: %s]\n", districts[i]);

        char buffer[256];
        ssize_t bytesRead;

        while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            printf("%s", buffer);
        }

        close(pipefd[0]);
        waitpid(pid, NULL, 0);
    }
    
}

//asta nu e necesara dar daca tot am terminat. 
void print_help() {
    printf("\nAvailable commands:\n");
    printf("\t-start_monitor\n");
    printf("\t-calculate_scores <district1> <district2> ...\n");
    printf("\t-help\n");
    printf("\t-exit\n\n");
}

int main() {
    char line[512]; //line buffer
    char *args[32]; //arguments to be read from user input

    printf("city_hub started.\n");
    print_help(); //ca sa printam doar la inceput nu la fiecare comanda.

    while (1) {
        printf("city_hub> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }

        int argc = split_command(line, args);

        if (argc == 0) {
            continue;
        }

        if (strcmp(args[0], "start_monitor") == 0) {
            start_monitor();
        }
        else if (strcmp(args[0], "calculate_scores") == 0) {
            calculate_scores_command(argc - 1, &args[1]);
        }
        else if (strcmp(args[0], "help") == 0) {
            print_help();
        }
        else if (strcmp(args[0], "exit") == 0) {
            printf("Exiting city_hub.\n");
            sleep(1); //ca sa fie mai fancy :)
            break;
        }
        else {
            printf("Unknown command: %s\n", args[0]);
            printf("Type 'help' to see available commands.\n");
        }

        while (waitpid(-1, NULL, WNOHANG) > 0) {
        }
    }

    return 0;
}