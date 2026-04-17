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

void add(char **args){
    fprintf(stderr, "<add> FUNCTION: WORK IN PROGRESS!\n\n");

    // argv[4]
    char FolderName[64];
    if (strlen(args[4]) > 63){
        fprintf(stderr, "Directory Name too long!\n");
        fprintf(stderr, "Exiting...\n");
        exit(-1);
    }
    strcpy(FolderName, args[4]);
    printf("Folder Name = %s\n", FolderName);

    char path[256];
    strcpy(path, "./");
    strcat(path, FolderName);

    if (dirExists(path)) {
        fprintf(stderr, "File already exists.\nWe have to append the input. Work in progress...\n");
    } else {
        fprintf(stderr, "Creating file...\n");

        char path[256];
        strcpy(path, "./");
        strcat(path, FolderName);
        if (mkdir(path, 0750) == -1){
            fprintf(stderr, "Error creating directory...");
            exit(-1);
        }

        //PENTU CREARE:
        int fd = -1;

        // reports.dat
        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/reports.dat");
        fd = open(path,O_CREAT | O_RDWR, 0664);
        if (fd == -1) perror("open reports.dat");
        else close(fd);

        // district.cfg
        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/district.cfg");
        fd = open(path,O_CREAT | O_RDWR,0640);
        if (fd == -1) perror("open district.cfg");
        else close(fd);

        // logged_district
        strcpy(path, "./");
        strcat(path, FolderName);
        strcat(path, "/logged_district");
        fd = open(path,O_CREAT | O_RDWR,0644);
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
    if (argc < 4){
        fprintf(stderr, "Not enough command line arguments.\n");
        fprintf(stderr, "2nd argument: --role\n3rd argument: Role Name\n4th argument: Command\n");
        exit(-1);
    }

    // printf("Current Role = %d\n", getRole(argv));
    displayRole(argv);
    printf("\n");
    selectCommand(argv);

    return 0;
}
