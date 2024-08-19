#pragma once
#include <stdio.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

void set_breakpoint(pid_t pid, void *addr);
