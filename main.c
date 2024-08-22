#include <stdio.h>
#include <sys/personality.h>
#include "src/cmd.h"
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

    printf("Welcome to Ydbg\n");
    printf("Ydbg> Want to get adresses to debug? type n if not: ");
    char yn = 0;
    scanf("%c", &yn);
    char cmd[SIZE] = {0};
    if(yn != NO)
    {
        printProgramAdresses(argv[1]);
    }


    

    return 0;
}
