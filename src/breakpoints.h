#pragma once
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <elf.h>
#include <fcntl.h>

#define BASE_SIZE 256

void set_breakpoint(pid_t pid, void *addr);

unsigned long get_base_addr(pid_t pid);
