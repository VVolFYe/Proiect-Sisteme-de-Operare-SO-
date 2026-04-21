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
void filter(char **args);


#endif