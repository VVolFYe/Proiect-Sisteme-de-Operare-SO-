#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <stdbool.h>

bool fileExists(const char *path);
bool dirExists(const char *path);
void appendToFile(const char *path, const char *text);

#endif