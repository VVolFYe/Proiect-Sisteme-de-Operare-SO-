#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include "utils/inputValidation.h"
#include "utils/fileFunctions.h"
#include "utils/commands.h"

void selectCommand(char **args){
    int selected = commandSelector(args);
    switch (selected){
        case 1:
            add(args);
            break;
        case 2:
            list(args);
            break;
        case 3:
            view(args);
            break;
        case 4:
            remove_report(args);
            break;
        case 5:
            update_threshold(args);
            break;
        case 6:
            filter(args);
            break;

        default:
        fprintf(stderr, "Unknown Command. Exiting...\n");
        exit(-1);
            
    }
}

int main(int argc, char **argv){
    if (argc < 6){
        fprintf(stderr, "Not enough command line arguments.\n");
        fprintf(stderr, "2nd argument: --role\n3rd argument: Role Name\n4th argument: --user\n5th argument: username\n6th argument: Command\n");
        exit(-1);
    }

    checkInput(argv);

    // printf("Current Role = %d\n", getRole(argv));
    displayRole(argv);
    printf("\n");
    selectCommand(argv);

    return 0;
}
