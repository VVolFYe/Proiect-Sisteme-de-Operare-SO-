#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "commands.h"
#include "fileFunctions.h"

void add(char **args){
    fprintf(stderr, "<add> FUNCTION: WORK IN PROGRESS!\n\n");

    char *district = args[6];

    if (strlen(district) > 63){
        fprintf(stderr, "Directory Name too long!\n");
        fprintf(stderr, "Exiting...\n");
        exit(-1);
    }

    checkDistrictExists(district);

    printf("District '%s' is ready.\n", district);

    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    printf("Reports file path: %s\n", reportsPath);

    // next step: append binary report here
}

void list(char **args){
    fprintf(stderr, "\n!<list> FUNCTION: TO BE IMPLEMENTED!\n");
}

void view(char **args){
    fprintf(stderr, "\n!<view> FUNCTION: TO BE IMPLEMENTED!\n");
}

void remove_report(char **args){
    fprintf(stderr, "\n!<remove_report> FUNCTION: TO BE IMPLEMENTED!\n");
}

void update_threshold(char **args){
    fprintf(stderr, "\n!<update_threshold> FUNCTION: TO BE IMPLEMENTED!\n");
}

void filter(char **args){
    fprintf(stderr, "\n!<function> FUNCTION: TO BE IMPLEMENTED!\n");
}