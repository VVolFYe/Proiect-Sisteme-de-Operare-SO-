#ifndef COMMANDS_H
#define COMMANDS_H

#include "report.h"

//utils for those
int getNextReportId(const char *reportsPath);
void readReportFromKeyboard(Report *r, const char *username, int id);
void appendReport(const char *reportsPath, Report *r);


void add(char **args);
void list(char **args);
void view(char **args);
void remove_report(char **args);
void update_threshold(char **args);

int match_condition(Report *r, const char *field, const char *op, const char *value);
int parse_condition(const char *input, char *field, char *op, char *value);
void filter(char **args);


#endif