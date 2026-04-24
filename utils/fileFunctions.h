#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

bool fileExists(const char *path);
bool dirExists(const char *path);
void appendToFile(const char *path, const char *text);
void buildDistrictPath(char *out, size_t outSize, const char *district);
void buildReportsPath(char *out, size_t outSize, const char *district);
void buildConfigPath(char *out, size_t outSize, const char *district);
void buildLogPath(char *out, size_t outSize, const char *district);
void checkDistrictExists(const char *district);
void modeToString(mode_t mode, char *out);
char *humanSize(uint64_t bytes);
void buildActiveReportsLinkPath(char *out, size_t outSize, const char *district);
void ensureActiveReportsSymlink(const char *district);
void logAction(const char *district, const char *role, const char *user, const char *action);

#endif