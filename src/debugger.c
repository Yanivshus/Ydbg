#include "debugger.h"
void debugger(pid_t p)
{
    int status;
    waitpid(p, &status, 0);
    // Continue the child process execution and handle signals.

    while (WIFSTOPPED(status)) {
        // Continue the process after it stops
        ptrace(PTRACE_CONT, p, NULL, NULL);

        // Wait for the child process to stop again
        waitpid(p, &status, 0);

        // Check if the child has exited or terminated
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
            break;
        }
    }

}

void run(char ** argv){
    // The child process.
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) { perror("ptrace"); exit(1);}
    kill(getpid(), SIGSTOP); // Stop the child process immediately
    execvp(argv[1], &argv[1]); // Execute the process passed as an argument
    perror("execvp"); // execvp only returns on error
    exit(1);
}