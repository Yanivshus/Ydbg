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
#include "cmd.h"

enum CMD {
    RUN=1,
    STEP=2,
    CON=3,
    BREAK=4,
    REGS=5,
    INS=6,
    QUIT=7
};

void run(char** argv);

void debugger(pid_t p);

void printProgramAdresses(const char* binary_path);

void doCommand(char* fullcmd);

//get enum that represent a command
enum CMD getIdByCommand(char** fullcmd);

int checkIfFileExists(const char* fname);
