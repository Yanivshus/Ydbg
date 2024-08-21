#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FLAGS 10

enum CMD {
    RUN=1,
    BREAK=2,
    CON=3
};


//for now implement without to much flags (10).
char** parsecmd(char* command);

//free a command.
void freeCmd(char** command);

int checkIfFileExists(const char* fname);