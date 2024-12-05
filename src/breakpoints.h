#pragma once
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#define BASE_SIZE 256

typedef struct{
    unsigned long addr;
    unsigned long original_data;
    bool is_enabled;
} breakpoint;



typedef struct{
    breakpoint ls_bp[100];
    int count;
} ls_bp;





void set_breakpoint(pid_t pid, unsigned long addr);

void remove_breakpoint(pid_t pid, unsigned long addr);

unsigned long get_base_addr(pid_t pid);
