#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "utils/report.h"

#define MAX_INSPECTORS 100

typedef struct {
    char inspector[USER_LEN];
    int score;
} InspectorScore;

int findInspector(InspectorScore scores[], int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(scores[i].inspector, name) == 0) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <district>\n", argv[0]);
        return 1;
    }

    char *district = argv[1];

    char reportsPath[256];
    snprintf(reportsPath, sizeof(reportsPath), "./%s/reports.dat", district);

    int fd = open(reportsPath, O_RDONLY);

    if (fd == -1) {
        printf("District [%s]: could not open reports.dat\n", district);
        return 1;
    }

    InspectorScore scores[MAX_INSPECTORS];
    int count = 0;

    Report r;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report)) {
        int pos = findInspector(scores, count, r.inspector);

        if (pos == -1) {
            if (count >= MAX_INSPECTORS) {
                printf("District [%s]: too many inspectors\n", district);
                close(fd);
                return 1;
            }

            strncpy(scores[count].inspector, r.inspector, USER_LEN - 1);
            scores[count].inspector[USER_LEN - 1] = '\0';
            scores[count].score = r.severity;
            count++;
        } else {
            scores[pos].score += r.severity;
        }
    }

    close(fd);

    if (count == 0) {
        printf("no reports found\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        printf("inspector= %s -> score= %d\n", scores[i].inspector, scores[i].score);
    }


    return 0;
}