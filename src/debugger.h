#pragma once

#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/user.h>

void run(char** argv);

void debugger(pid_t p);

void printProgramAdresses(pid_t p);