#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FLAGS 10


//parsing a command by ' ' delimeter and reterning array of strings.
char** parsecmd(char* command);

//free a command.
void freeCmd(char** command);

//check if file exists on disk.
int checkIfFileExists(const char* fname);


