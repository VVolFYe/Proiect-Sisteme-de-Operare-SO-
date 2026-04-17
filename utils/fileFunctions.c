#include <sys/stat.h>
#include <stdbool.h>
#include "fileFunctions.h"

bool fileExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

bool dirExists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}