#include "breakpoints.h"
#include <string.h>

void set_breakpoint(pid_t pid, unsigned long addr)
{
    // Read original word at addr
    long orig_word = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    printf("%ld\n", orig_word);
    if (orig_word == -1 && errno !=0)
    {
        perror("ptrace PEEKTEXT failed");
        return;
    }

    // Replace the least significant byte with INT 3 (0xCC)
    long int3_word = (orig_word & ~0xFF) | 0xCC;
    if (ptrace(PTRACE_POKEDATA, pid, addr, int3_word) == -1)
    {
        perror("ptrace POKETEXT failed");
        return;
    }
    
}

unsigned long get_base_addr(pid_t pid)
{
    char filename[256];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);

    FILE *maps_file = fopen(filename, "r");
    if (!maps_file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char line[256];
    unsigned long base_address = 0;

    // Read through the maps file line by line
    while (fgets(line, sizeof(line), maps_file)) {
        // Check if the line corresponds to an executable segment
        if (strstr(line, "r--p") != NULL) {
            // Extract the base address from the first column
            sscanf(line, "%lx", &base_address);
            break;
        }
    }

    fclose(maps_file);
    return base_address;

}