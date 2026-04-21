#ifndef REPORT_H
#define REPORT_H

#include <time.h>

#define USER_LEN 64
#define CATEGORY_LEN 32
#define DESC_LEN 256

typedef struct Report{
    int id;
    char inspector[USER_LEN];
    double latitude;
    double longitude;
    char category[CATEGORY_LEN];
    int severity;
    time_t timestamp;
    char description[DESC_LEN];
} Report;

#endif