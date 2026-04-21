#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include "commands.h"
#include "fileFunctions.h"
#include "report.h"

int getNextReportId(const char *reportsPath) {
    struct stat st;

    if (stat(reportsPath, &st) == -1) {
        perror("stat reports.dat");
        exit(-1);
    }

    return (st.st_size / sizeof(Report)) + 1;
}

void readReportFromKeyboard(Report *r, const char *username, int id) {
    memset(r, 0, sizeof(Report));

    r->id = id;

    strncpy(r->inspector, username, USER_LEN - 1);

    printf("Latitude: ");
    scanf("%lf", &r->latitude);

    printf("Longitude: ");
    scanf("%lf", &r->longitude);

    printf("Category: ");
    scanf("%31s", r->category);

    printf("Severity (1-3): ");
    scanf("%d", &r->severity);

    if (r->severity < 1 || r->severity > 3) {
        fprintf(stderr, "Severity must be between 1 and 3.\n");
        exit(-1);
    }

    getchar();

    printf("Description: ");
    fgets(r->description, DESC_LEN, stdin);
    r->description[strcspn(r->description, "\n")] = '\0';

    r->timestamp = time(NULL);
}

void appendReport(const char *reportsPath, Report *r) {
    int fd = open(reportsPath, O_WRONLY | O_APPEND);

    if (fd == -1) {
        perror("open reports.dat");
        exit(-1);
    }

    if (write(fd, r, sizeof(Report)) != sizeof(Report)) {
        perror("write report");
        close(fd);
        exit(-1);
    }

    close(fd);
}

void add(char **args){
    // fprintf(stderr, "<add> FUNCTION\n\n");

    char *district = args[6];
    char *username = args[4];

    if (strlen(district) > 63){
        fprintf(stderr, "Directory Name too long!\n");
        fprintf(stderr, "Exiting...\n");
        exit(-1);
    }

    checkDistrictExists(district);

    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    int id = getNextReportId(reportsPath);

    Report r;
    memset(&r, 0, sizeof(Report));

    readReportFromKeyboard(&r, username, id);

    appendReport(reportsPath, &r);

    printf("Report added successfully with ID %d.\n", id);
}

void list(char **args){
    fprintf(stderr, "\n!<list> FUNCTION: ONGOING!\n");

    char *district = args[6];
    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    if (!fileExists(reportsPath)) {
        fprintf(stderr, "Path for district [%s] not found\n", district);
        exit(-1);
    }

    struct stat st;
    if (stat(reportsPath, &st) == -1) {
        perror("stat reports.dat");
        exit(-1);
    }

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