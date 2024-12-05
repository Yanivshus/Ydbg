#include <stdio.h>
#include <sys/personality.h>
#include "src/debugger.h"
#define NO 'n'
#define SIZE 100

int main(int argc, char** argv)
{
    printf("Welcome to Ydbg!\nEnter help to see commands.\n");



    //check if program used correctly.
    if(argc < 2) { fprintf(stderr, "Usage: ./%s <name of program to debug> \n", argv[0]); exit(1); }
    //check if file provided to debug exists.
    if(checkIfFileExists(argv[1]) == 0){ fprintf(stderr, "File %s doesn't exists\n", argv[1]); exit(1); }


    char input[100] = {0};

    pid_t p = fork();
    if(p == 0)
    {
        // Disable ASLR for this process so all adrreses remain consistent.
            const int old_personality = personality(ADDR_NO_RANDOMIZE);
            if (!(old_personality & ADDR_NO_RANDOMIZE)) {
                const int new_personality = personality(ADDR_NO_RANDOMIZE);
                if (new_personality & ADDR_NO_RANDOMIZE) {
                       ptrace(PTRACE_TRACEME, 0, NULL, NULL);
                        kill(getpid(), SIGSTOP);
                        execvp(argv[1], &argv[1]);
                        perror("execvp");  
                        exit(1);
                }
            }
        

     
    }
    else if(p > 0)
    {
        int status;
        
        waitpid(p, &status, 0);  // Wait for the child to stop
        printf("Running %s on pid: %d.\n", argv[1], p);

        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            fprintf(stderr, "Error: Child process terminated unexpectedly.\n");
            return 1;
        }
       
        while(1)
        {

            if (WIFSTOPPED(status) || WSTOPSIG(status) == SIGTRAP) 
            {
                int should_wait = 0;
                printf("Ygdb> ");
                if(fgets(input, sizeof(input), stdin) != NULL && input[0] != '\n')
                {
                    int len = strlen(input);
                    if(len > 0)
                    { 
                        input[len - 1] = '\0'; 
                    }
                    if(WSTOPSIG(status) == SIGTRAP) // restore breakpoint.
                    {

                    }

                    should_wait = doCommand(input,p,argv[1]);
                }

                if(should_wait == 1)
                {
                    waitpid(p, &status, 0);
                }
                else if(should_wait == 2){
                    printf("Error processing given address");
                }
            }
            
            // exit if program exited.
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                printf("Child process exited with status %d.\n", WEXITSTATUS(status));
                break;
            }
        }
    }
    return 0;
}



