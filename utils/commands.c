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

    char FolderName[64];
    if (strlen(args[6]) > 63){
        fprintf(stderr, "Directory Name too long!\n");
        fprintf(stderr, "Exiting...\n");
        exit(-1);
    }

    strcpy(FolderName, args[6]);
    printf("Folder Name = %s\n", FolderName);

    char path[256];
    strcpy(path, "./");
    strcat(path, FolderName);

    if (dirExists(path)) {
        //file exists
        fprintf(stderr, "File already exists.\nWe have to append the input. Work in progress...\n");
    } else {
        //file doesn't exists

        fprintf(stderr, "Creating file...\n");

        if (mkdir(path, 0750) == -1){
            fprintf(stderr, "Error creating directory...");
            exit(-1);
        }

        int fd = -1;

        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/reports.dat");
        fd = open(path, O_CREAT | O_RDWR, 0664);
        chmod(path, 0664);
        if (fd == -1) perror("open reports.dat");
        else close(fd);

        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/district.cfg");
        fd = open(path, O_CREAT | O_RDWR, 0640);
        chmod(path, 0640);
        if (fd == -1) perror("open district.cfg");
        else close(fd);

        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/logged_district");
        fd = open(path, O_CREAT | O_RDWR, 0644);
        chmod(path, 0644);
        if (fd == -1) perror("open logged_district");
        else close(fd);
    }
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