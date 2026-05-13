#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

void start_monitor(){
    // pid_t hub_mon = fork();
    int pipefd[2];
    pipefd[0] = -1;
    pipefd[1] = -1;


    if (pipe(pipefd) == -1){
        fprintf(stderr, "Pipe error [city_hub.c].\n");
        exit(-1);
    }

    dup2(pipefd[1], STDOUT_FILENO); //aici facem legatura

    int pid = fork();

    if (pid == 0){ //child:
        execlp("./monitor_reports", "./monitor_reports", NULL);
    } else if (pid == -1){
        fprintf(stderr, "Eroare fork process.\n");
        exit(-1);
    } else{
        waitpid(pid, NULL, 0); //asteptam.
    }

}

int main(int argc, char *argv[]){
    start_monitor();

    return 0;
}