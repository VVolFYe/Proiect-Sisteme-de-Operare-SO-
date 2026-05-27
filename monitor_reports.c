#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


volatile sig_atomic_t isAlive = 1;

void handleSIGINT(int sig){
    (void)sig; //era cv warning la un moment dat daca puneam asa se oprea
    isAlive = 0;

    char msg[] = "DONE: monitor_reports received SIGINT. Exiting.\n";

    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

//handle for sigusr when creating a file 
void handleSIGUSR1(int sig){
    (void)sig;
    char msg[] = "INFO: A new report has been added.\n";

    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

void initialize(){
    int oldFd = open(".monitor_pid", O_RDONLY);

    if (oldFd != -1) {
        char existingPid[64];
        ssize_t n = read(oldFd, existingPid, sizeof(existingPid) - 1);
        close(oldFd);

        if (n > 0) {
            existingPid[n] = '\0';
            char msg[128];
            snprintf(msg, sizeof(msg), "ERROR: monitor already running with PID %s", existingPid);
            write(STDOUT_FILENO, msg, strlen(msg));
        } else {
            char msg[] = "ERROR: monitor already running, but PID could not be read.\n";
            write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        }

        exit(-1);
    }


    pid_t monitorPid = getpid();
    char pid[128];
    snprintf(pid, sizeof(pid), "%d\n", monitorPid);
    
    int fd = -1;
    if ((fd = open(".monitor_pid", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1){
        fprintf(stderr, "Error creating .monitor_pid file.\n");
        exit(-1);
    }
    write(fd, pid, strlen(pid));
    close(fd);
}

void end() {
    if (unlink(".monitor_pid") == -1) {
        fprintf(stderr, "failed to unlink.\n");
        exit(-1);
    }

    char msg[] = "DONE: monitor_pid file removed.\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}


//handler from stackoverflow
static void pSigHandler(int signo){
    switch (signo) {
            case SIGINT:
                handleSIGINT(signo);
                // fflush(stdout); //i took it from a stack overflow i see it works without it too
                break;
            case SIGUSR1:
                handleSIGUSR1(signo);
                // fflush(stdout); //i took it from a stack overflow i see it works without it too
                break;
            default:
                break;
    }
}


int main(void){
    initialize();

    fprintf(stderr, "monitor_reports: Ongoing.\n");

    struct sigaction psa;
    memset (&psa, 0, sizeof (psa));
    sigemptyset(&psa.sa_mask); //sa fim safe

    psa.sa_flags = SA_RESTART;
    psa.sa_handler = pSigHandler;
    
    if (sigaction(SIGUSR1, &psa, NULL) == -1) {
        perror("sigaction SIGUSR1");
        exit(EXIT_FAILURE);
    }
    
    if (sigaction(SIGINT, &psa, NULL) == -1) {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    while (isAlive == 1){
        pause();
    }
    end();

    return 0;
}