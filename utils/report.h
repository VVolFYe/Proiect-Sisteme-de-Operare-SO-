#ifndef REPORT_H
#define REPORT_H

#include <time.h>

#define USER_LEN 64
#define CATEGORY_LEN 32
#define DESC_LEN 256

typedef struct {
    int id;
    char inspector[64];
    double latitude;
    double longitude;
    char category[32];
    int severity;
    time_t timestamp;
    char description[256];
} Report;

#endif