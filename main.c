#include <stdio.h>
#include "src/cmd.h"
#include "src/debugger.h"

int main(int argc, char** argv)
{
    printf("Welcome to Ydbg\n");

    //check if program used correctly.
    if(argc < 2) { fprintf(stderr, "Usage: ./%s <name of program to debug> \n", argv[0]); exit(1); }
    //check if file provided to debug exists.
    if(checkIfFileExists(argv[1]) == 0){ fprintf(stderr, "File %s doesn't exists\n", argv[1]); exit(1); }

    pid_t p = fork();

    if(p == 0)
    {
        run(argv);
    }
    else{
        debugger(p);
    }
    

    return 0;
}
