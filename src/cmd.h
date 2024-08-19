#include <string.h>
#include <stdio.h>


enum CMD {
    RUN=1,
    BREAK=2,
    CON=3
};


//for now implement without to much flags.
enum CMD parsecmd(char* command);