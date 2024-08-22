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
#include <stdint.h>
#include <elf.h>
#include <fcntl.h>
#include <string.h>

void run(char** argv);

void debugger(pid_t p);

void printProgramAdresses(const char* binary_path);