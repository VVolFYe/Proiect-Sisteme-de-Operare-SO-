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


void buildDistrictPath(char *out, size_t outSize, const char *district) {
    snprintf(out, outSize, "./%s", district);
}

void buildReportsPath(char *out, size_t outSize, const char *district) {
    snprintf(out, outSize, "./%s/reports.dat", district);
}

void buildConfigPath(char *out, size_t outSize, const char *district) {
    snprintf(out, outSize, "./%s/district.cfg", district);
}

void buildLogPath(char *out, size_t outSize, const char *district) {
    snprintf(out, outSize, "./%s/logged_district", district);
}

void checkDistrictExists(const char *district) {
    char districtPath[256];
    char reportsPath[256];
    char cfgPath[256];
    char logPath[256];

    buildDistrictPath(districtPath, sizeof(districtPath), district);
    buildReportsPath(reportsPath, sizeof(reportsPath), district);
    buildConfigPath(cfgPath, sizeof(cfgPath), district);
    buildLogPath(logPath, sizeof(logPath), district);

    if (!dirExists(districtPath)) {
        if (mkdir(districtPath, 0750) == -1) {
            perror("mkdir district");
            exit(-1);
        }
    }

    chmod(districtPath, 0750);

    int fd;

    if (!fileExists(reportsPath)) {
        fd = open(reportsPath, O_CREAT | O_RDWR, 0664);
        if (fd == -1) {
            perror("open reports.dat");
            exit(-1);
        }
        close(fd);
    }
    chmod(reportsPath, 0664);

    if (!fileExists(cfgPath)) {
        fd = open(cfgPath, O_CREAT | O_RDWR, 0640);
        if (fd == -1) {
            perror("open district.cfg");
            exit(-1);
        }

        const char *defaultCfg = "threshold=3\n";
        if (write(fd, defaultCfg, strlen(defaultCfg)) == -1) {
            perror("write district.cfg");
            close(fd);
            exit(-1);
        }

        close(fd);
    }
    chmod(cfgPath, 0640);

    if (!fileExists(logPath)) {
        fd = open(logPath, O_CREAT | O_RDWR, 0644);
        if (fd == -1) {
            perror("open logged_district");
            exit(-1);
        }
        close(fd);
    }
    chmod(logPath, 0644);
}

