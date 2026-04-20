#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "inputValidation.h"

void checkInput(char **args){
    if (strcmp(args[1], "--role") != 0){
        fprintf(stderr, "1st argument must be: --role");
        exit(-1);
    }

    if (strcmp(args[3], "--user") != 0){
        fprintf(stderr, "2nd argument must be: --role");
        exit(-1);
    }

    if (strlen(args[4]) > 63){
        fprintf(stderr, "username too long.\n");
        exit(-1);
    }
}


// -1 error, 1 manager, 2 inspector 
int getRole(char **args){
    if (strcmp(args[1], "--role") != 0){
        fprintf(stderr, "Second argument must be --role\n");
        exit(-1);
    }

    if (strcmp(args[2], "manager") == 0){
        return 1;
    }

    if (strcmp(args[2], "inspector") == 0){
        return 2;
    }

    fprintf(stderr, "Unknown Role Name\n");
    return -1; //must be other ot some error
}

char *getUser(char **args){
    return args[4];
}

void displayRole(char **args){
    int roleNumber = getRole(args);
    switch(roleNumber){
        case 1:
            printf("Current role = Manager\n"); break;
        case 2:
            printf("Current role = Inspector\n"); break;
        default:
            exit(-1);
    }
}


/*
    Command ID:
    error = -1
    add = 1
    list = 2
    view = 3
    remove_report = 4 
    update_threshold = 5 
    filter = 6
*/
int commandSelector(char **args){
    if ((strcmp(args[5], "--add") == 0) || (strcmp(args[5], "add") == 0)){
        return 1;
    }
    if ((strcmp(args[5], "--list") == 0) || (strcmp(args[5], "list") == 0)){
        return 2;
    }
    if ((strcmp(args[5], "--view") == 0) || (strcmp(args[5], "view") == 0)){
        return 3;
    }
    if ((strcmp(args[5], "--remove_report") == 0) || (strcmp(args[5], "remove_report") == 0)){
        return 4;
    }
    if ((strcmp(args[5], "--update_threshold") == 0) || (strcmp(args[5], "update_threshold") == 0)){
        return 5;
    }
    if ((strcmp(args[5], "--filter") == 0) || (strcmp(args[5], "filter") == 0)){
        return 6;
    }
    return -1;
}
