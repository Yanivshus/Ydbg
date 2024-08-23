#include <stdio.h>
#include <sys/personality.h>
#include "src/debugger.h"
#define NO 'n'
#define SIZE 100

int main(int argc, char** argv)
{
    // Disable ASLR for this process so all adrreses remain consistent.
    unsigned long current_personality = personality(0xffffffff);
    personality(current_personality | ADDR_NO_RANDOMIZE);

    //check if program used correctly.
    if(argc < 2) { fprintf(stderr, "Usage: ./%s <name of program to debug> \n", argv[0]); exit(1); }
    //check if file provided to debug exists.
    if(checkIfFileExists(argv[1]) == 0){ fprintf(stderr, "File %s doesn't exists\n", argv[1]); exit(1); }


    char input[100] = {0};
    while(1)
    {
        printf("Ygdb> ");
        fgets(input, sizeof(input), stdin);
        input[strlen(input) - 1] = '\0';

        doCommand(input);
    }

    return 0;
}

void printHelp(){
    printf("    Help menu: \n");
    printf("    s - step\n");
    printf("    c - continue run\n");
    printf("    b <ADDR> - set breakpoint at address\n");
    printf("    regs - watch cpu registers.\n");
    printf("    ins - inspect symbol table and addresses.\n");
    printf("    quit - exit program.\n");
}

