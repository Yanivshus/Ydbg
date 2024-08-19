#include "breakpoints.h"

void set_breakpoint(pid_t pid, void *addr)
{
    // Read original byte at addr
    uint8_t orig_byte = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);

    // Replace it with INT 3 (0xCC)
    uint8_t int3 = 0xCC;
    ptrace(PTRACE_POKEDATA, pid, addr, (orig_byte & 0xFFFFFF00) | int3);
}