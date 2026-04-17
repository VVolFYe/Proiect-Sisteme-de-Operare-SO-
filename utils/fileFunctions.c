#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "fileFunctions.h"

bool fileExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

bool dirExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

void appendToFile(const char *path, const char *text) {
    int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        fprintf(stderr, "Error when writing in the file: %s (error 1)\n", path);
        exit(-1);
    }

    if (write(fd, text, strlen(text)) == -1) {
        fprintf(stderr, "Error when writing in the file: %s (error 2)\n", path);
        close(fd);
        exit(-1);
    }

    close(fd);
}