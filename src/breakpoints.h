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

#define BASE_SIZE 256

typedef struct {
    unsigned long addr;
    uint8_t orig_byte;
} Breakpoint;

void set_breakpoint(pid_t pid, unsigned long addr);

unsigned long get_base_addr(pid_t pid);
