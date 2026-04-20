#ifndef INPUTVALIDATION_H
#define INPUTVALIDATION_H

void checkInput(char **args);
int getRole(char **args);
void displayRole(char **args);
int commandSelector(char **args);
char *getUser(char **args);
void checkPermissions(char **args);


#endif