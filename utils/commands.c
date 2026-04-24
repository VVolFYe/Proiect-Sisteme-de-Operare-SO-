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
    int fd = open(reportsPath, O_RDONLY);
    if (fd == -1) {
        perror("open reports.dat");
        exit(-1);
    }

    Report r;
    int maxId = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        if (r.id > maxId) {
            maxId = r.id;
        }
    }

    close(fd);
    return maxId + 1; //i chose this one because we want to be sure that we don't get the same ID two time ig.
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

int parse_condition(const char *input, char *field, char *op, char *value){
    const char *p1 = strchr(input, ':');
    if (p1 == NULL) return 0;

    const char *p2 = strchr(p1 + 1, ':');
    if (p2 == NULL) return 0;

    int fieldLen = p1 - input;
    int opLen = p2 - (p1 + 1);
    int valueLen = strlen(p2 + 1);

    if (fieldLen <= 0 || opLen <= 0 || valueLen <= 0) return 0;

    strncpy(field, input, fieldLen);
    field[fieldLen] = '\0';

    strncpy(op, p1 + 1, opLen);
    op[opLen] = '\0';

    strcpy(value, p2 + 1);

    return 1;
}

int match_condition(Report *r, const char *field, const char *op, const char *value){
    if (strcmp(field, "severity") == 0){
        int v = atoi(value);

        if (strcmp(op, "==") == 0) return r->severity == v;
        if (strcmp(op, "!=") == 0) return r->severity != v;
        if (strcmp(op, "<")  == 0) return r->severity <  v;
        if (strcmp(op, "<=") == 0) return r->severity <= v;
        if (strcmp(op, ">")  == 0) return r->severity >  v;
        if (strcmp(op, ">=") == 0) return r->severity >= v;

        return 0;
    }

    if (strcmp(field, "category") == 0){
        if (strcmp(op, "==") == 0) return strcmp(r->category, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(r->category, value) != 0;
        return 0;
    }

    if (strcmp(field, "inspector") == 0){
        if (strcmp(op, "==") == 0) return strcmp(r->inspector, value) == 0;
        if (strcmp(op, "!=") == 0) return strcmp(r->inspector, value) != 0;
        return 0;
    }

    if (strcmp(field, "timestamp") == 0){
        long long v = atoll(value);
        long long t = (long long)r->timestamp;

        if (strcmp(op, "==") == 0) return t == v;
        if (strcmp(op, "!=") == 0) return t != v;
        if (strcmp(op, "<")  == 0) return t <  v;
        if (strcmp(op, "<=") == 0) return t <= v;
        if (strcmp(op, ">")  == 0) return t >  v;
        if (strcmp(op, ">=") == 0) return t >= v;

        return 0;
    }

    return 0;
}

void add(char **args){

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

    char date[64];
    strftime(date, sizeof(date), "%d-%m-%Y %H:%M:%S", localtime(&(st_district.st_mtime)));
    printf("[reports.dat] --> last date modified: [%s]\n", date);

    int fd = open(reportsPath, O_RDONLY);
    if (fd == -1){
        perror("open reports.dat");
        exit(-1);
    }

    int found = 0;
    Report r;
    
    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){
        found = 1;

        printf("\n-----------------------------\n");
        printf("ID: %d\n", r.id);
        printf("Inspector: %s\n", r.inspector);
        printf("Latitude: %.2lf\n", r.latitude);
        printf("Longitude: %.2lf\n", r.longitude);
        printf("Category: %s\n", r.category);
        printf("Severity: %d\n", r.severity);
        printf("Timestamp: %s", ctime(&(r.timestamp)));
        printf("Description: %s\n", r.description);
    }

    if (found == 0){
        fprintf(stderr, "No report was found in the [%s] district.\n", district);
        close(fd);
        return;
    }

    close(fd);
}
void view(char **args){

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
        perror("open reports.dat");
        exit(-1);
    }

    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){
        if (searchedID == r.id){
            found = 1;
            printf("ID: %d\n", r.id);
            printf("Inspector: %s\n", r.inspector);
            printf("Latitude: %.2lf\n", r.latitude);
            printf("Longitude: %.2lf\n", r.longitude);
            printf("Category: %s\n", r.category);
            printf("Severity: %d\n", r.severity);
            printf("Timestamp: %s", ctime(&(r.timestamp)));
            printf("Description: %s\n", r.description);
            break;
        }
    }

    if (found == 0){
        fprintf(stderr, "Report with ID [%d] doesn't exist in district [%s]\n", searchedID, district);
        close(fd);
        return;
    }

    close(fd);
}

void remove_report(char **args){

    int currentRole = getRole(args);
    if (currentRole != 1){
        fprintf(stderr, "Functionality available only for managers.\n");
        exit(-1);
    }

    char *district = args[6];
    int searchedID = atoi(args[7]);

    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    if (!fileExists(reportsPath)) {
        fprintf(stderr, "Path for district [%s] not found\n", district);
        exit(-1);
    }

    int fd = open(reportsPath, O_RDWR);
    if (fd == -1){
        perror("open reports.dat");
        exit(-1);
    }

    Report r;
    off_t pos = 0;
    off_t foundPos = -1;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){
        if (r.id == searchedID){
            foundPos = pos;
            break;
        }
        pos += sizeof(Report);
    }

    if (foundPos == -1){
        fprintf(stderr, "Report with ID [%d] doesn't exist in district [%s]\n", searchedID, district);
        close(fd);
        return;
    }

    off_t nextPos = foundPos + sizeof(Report);
    Report temp;

    while (1){
        if (lseek(fd, nextPos, SEEK_SET) == -1){
            perror("lseek read");
            close(fd);
            exit(-1);
        }

        ssize_t bytesRead = read(fd, &temp, sizeof(Report));
        if (bytesRead == -1){
            perror("read shift");
            close(fd);
            exit(-1);
        }

        if (bytesRead == 0){
            break;
        }

        if (bytesRead != sizeof(Report)){
            fprintf(stderr, "Corrupted report file.\n");
            close(fd);
            exit(-1);
        }

        if (lseek(fd, nextPos - sizeof(Report), SEEK_SET) == -1){
            perror("lseek write");
            close(fd);
            exit(-1);
        }

        if (write(fd, &temp, sizeof(Report)) != sizeof(Report)){
            perror("write shift");
            close(fd);
            exit(-1);
        }

        nextPos += sizeof(Report);
    }

    struct stat st;
    if (fstat(fd, &st) == -1){
        perror("fstat");
        close(fd);
        exit(-1);
    }

    if (ftruncate(fd, st.st_size - sizeof(Report)) == -1){
        perror("ftruncate");
        close(fd);
        exit(-1);
    }

    close(fd);

    printf("Report with ID [%d] was removed from district [%s].\n", searchedID, district);
}

void update_threshold(char **args){

    int currentRole = getRole(args);
    if (currentRole != 1){
        fprintf(stderr, "Functionality available only for managers.\n");
        exit(-1);
    }

    char *district = args[6];
    int newValue = atoi(args[7]);

    if (newValue < 1 || newValue > 3){
        fprintf(stderr, "Threshold must be between 1 and 3.\n");
        exit(-1);
    }

    char cfgPath[256];
    buildConfigPath(cfgPath, sizeof(cfgPath), district);

    if (!fileExists(cfgPath)) {
        fprintf(stderr, "Configuration file for district [%s] not found.\n", district);
        exit(-1);
    }

    struct stat st;
    if (stat(cfgPath, &st) == -1){
        perror("stat district.cfg");
        exit(-1);
    }

    if ((st.st_mode & 0777) != 0640){
        fprintf(stderr, "Refusing to update threshold: district.cfg permissions are [%o], expected [640].\n", st.st_mode & 0777);
        exit(-1);
    }

    int fd = open(cfgPath, O_WRONLY | O_TRUNC);
    if (fd == -1){
        perror("open district.cfg");
        exit(-1);
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "threshold=%d\n", newValue);

    if (write(fd, buffer, strlen(buffer)) == -1){
        perror("write district.cfg");
        close(fd);
        exit(-1);
    }

    close(fd);

    printf("Threshold for district [%s] was updated to %d.\n", district, newValue);
}

void filter(char **args){

    char *district = args[6];

    char reportsPath[256];
    buildReportsPath(reportsPath, sizeof(reportsPath), district);

    if (!fileExists(reportsPath)) {
        fprintf(stderr, "Path for district [%s] not found\n", district);
        exit(-1);
    }

    int fd = open(reportsPath, O_RDONLY);
    if (fd == -1){
        perror("open reports.dat");
        exit(-1);
    }

    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)){
        int allMatch = 1;

        //we itterate throught all the condition from commandline arguments
        for (int i = 7; args[i] != NULL; i++){
            char field[64], op[16], value[128];

            //we parse them
            if (!parse_condition(args[i], field, op, value)){
                fprintf(stderr, "Invalid condition format: [%s]\n", args[i]);
                close(fd);
                exit(-1);
            }

            //if any of them from each report doesnt match the conditions we set allMatch to false
            if (!match_condition(&r, field, op, value)){
                allMatch = 0;
                break;
            }
        }

        if (allMatch){
            found = 1;
            printf("\n-----------------------------\n");
            printf("ID: %d\n", r.id);
            printf("Inspector: %s\n", r.inspector);
            printf("Latitude: %.2lf\n", r.latitude);
            printf("Longitude: %.2lf\n", r.longitude);
            printf("Category: %s\n", r.category);
            printf("Severity: %d\n", r.severity);
            printf("Timestamp: %s", ctime(&(r.timestamp)));
            printf("Description: %s\n", r.description);
        }
    }

    if (!found){
        fprintf(stderr, "No matching reports found in district [%s]\n", district);
    }

    close(fd);
}