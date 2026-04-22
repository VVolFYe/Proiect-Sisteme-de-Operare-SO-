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
#include "inputValidation.h"

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
    r->description[strcspn(r->description, "\n")] = '\0'; //eliminate \n with strcpsn

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
    char *district = args[6];
    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    if (!fileExists(reportsPath)) {
        fprintf(stderr, "Path for district [%s] not found\n", district);
        exit(-1);
    }

    struct stat st_district;
    if (stat(reportsPath, &st_district) == -1) {
        perror("stat reports.dat");
        exit(-1);
    }
    
    printf("Information about reports.dat:\n");

    char perms[10];
    modeToString(st_district.st_mode, perms);
    printf("[reports.dat] --> file permissions: [%s]\n", perms);

    uint64_t bytes = st_district.st_size;
    printf("[reports.dat] --> file size: [%s]\n", humanSize(bytes));

    char date[30];
    strftime(date, 30, "%d-%m-%y", gmtime(&(st_district.st_mtime)));
    printf("[reports.dat] --> last date modified: [%s]\n", date);

    int fd = open(reportsPath, O_RDONLY);
    if (fd == -1){
        perror(NULL);
        exit(-1);
    }

    int found = 0;
    Report r;
    
    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){
        found = 1;
        printf("ID: %d\n", r.id);
        printf("Inspector: %s\n", r.inspector);
        printf("Category: %s\n", r.category);
        printf("Severity: %d\n", r.severity);
        printf("Description: %s\n", r.description);
    }

    if (found == 0){fprintf(stderr, "No report was found in the [%s] district.\n", district); return;}
}

void view(char **args){
    fprintf(stderr, "<view> FUNCTION: ONGOING\n");

    char *district = args[6];
    int searchedID = atoi(args[7]);

    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    if (!fileExists(reportsPath)) {
        fprintf(stderr, "Path for district [%s] not found\n", district);
        exit(-1);
    }


    int fd = open(reportsPath, O_RDONLY);
    if (fd == -1){
        perror(NULL);
        exit(-1);
    }

    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){

        if (searchedID == r.id){
            found = 1;
            printf("ID: %d\n", r.id);
            printf("Inspector: %s\n", r.inspector);
            printf("Category: %s\n", r.category);
            printf("Severity: %d\n", r.severity);
            printf("Description: %s\n", r.description);
        }
    }

    if (found == 0){fprintf(stderr, "Report with ID [%d] doesn't exist in district [%s]\n", searchedID, district); return;}
}

void remove_report(char **args){
    fprintf(stderr, "\n!<remove_report> FUNCTION: ONGOING\n");
    
    //kick out if not manager
    int currentRole = getRole(args);
    if (currentRole != 1){
        fprintf(stderr, "Functionality available only for managers. ;)\n");
        exit(-1);
    }


}

void update_threshold(char **args){
    fprintf(stderr, "\n!<update_threshold> FUNCTION: TO BE IMPLEMENTED!\n");

    //kick out if not manager
    int currentRole = getRole(args);
    if (currentRole != 1){
        fprintf(stderr, "Functionality available only for managers. ;)\n");
        exit(-1);
    }


}

void filter(char **args){
    fprintf(stderr, "\n!<function> FUNCTION: TO BE IMPLEMENTED!\n");
}