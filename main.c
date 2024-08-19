#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char** argv)
{
    printf("Welcome to Ydbg\n");
    if(argc < 2)
        fprintf(stderr, "Usage: ./%s <name of program to debug> \n", argv[0]);

    pid_t p = fork();
    //check if child process created.
    if(p < 0){
        fprintf(stderr, "Problem creating child procces!\nexiting...\n");
        exit(1);
    }
    else if(p == 0){
        //tells to the krenel that the parent process willl controll the child.
        ptrace(PTRACE_TRACEME);
        kill(getpid(), SIGSTOP); // Stop the child process immediately
        execvp(argv[1], &argv[1]); // execute the process passed as an argument
    }
    else{
        int status, syscall, retval;
        waitpid(p,&status, 0);
    }


}