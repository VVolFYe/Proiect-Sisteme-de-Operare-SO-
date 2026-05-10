#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <string.h>


volatile sig_atomic_t isAlive = 1;

void handleSIGINT(int sig){
    isAlive = 0;
    char messageSIGINT[128];
    messageSIGINT[0] = '\0';
    strcpy(messageSIGINT, "Monitor_reports: Received SIGINT. Exiting...\n");
    write(STDOUT_FILENO, messageSIGINT, strlen(messageSIGINT));
}

//handle for sigusr when creating a file 
void handleSIGUSR1(int sig){
    char messageSIGUSR1[128];
    messageSIGUSR1[0] = '\0';
    strcpy(messageSIGUSR1, "Monitor_reports: Received SIGUSR1. A new report has been added\n");
    write(STDOUT_FILENO, messageSIGUSR1, strlen(messageSIGUSR1));
}

void initialize(){
    pid_t monitorPid = getpid();
    char pid[128];
    snprintf(pid, 128, "%d", monitorPid);

    int fd = -1;
    if ((fd = open(".monitor_pid", O_WRONLY | O_CREAT, 0644)) == -1){
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
    printf("monitor_pid killed.\n");
}

//handler from stackoverflow
static void pSigHandler(int signo){
    switch (signo) {
            case SIGINT:
                handleSIGINT(signo);
                fflush(stdout);
                break;
            case SIGUSR1:
                handleSIGUSR1(signo);
                fflush(stdout);
                break;
            default:
                break;
    }
}

int main(int argc, char *argv[]){
    fprintf(stderr, "monitor_reports: Ongoing.\n");

    struct sigaction psa;
    memset (&psa, 0, sizeof (psa));
    sigemptyset(&psa.sa_mask); //sa fim safe

    psa.sa_flags = SA_RESTART;
    psa.sa_handler = pSigHandler;

    sigaction(SIGUSR1, &psa, NULL);
    sigaction(SIGINT, &psa, NULL);
    // sigaction(SIGUSR1, &psa, NULL);

    initialize();
    while (isAlive == 1){
        pause();
    }
    end();

    return 0;
}