#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "utils/inputValidation.h"
#include "utils/fileFunctions.h"
#include "utils/commands.h"
#include "utils/report.h"

void scorer(char *districtName){
    printf("scorer function to be implemented [%s].\n", districtName);
    char command[256];
    snprintf(command, sizeof(command), "./scorer.sh %s", districtName);
    execlp("./scorer.sh", "./scorer.sh", districtName, NULL);
}

void calculate_scores(int argc, char *args[]){
    
    
    for(int i = 1; i < argc; i++){
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(-1);
        }

        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        scorer(args[i]);
    }
}

int main(int argc, char *argv[]){

    calculate_scores(argc, argv);

    return 0;
}