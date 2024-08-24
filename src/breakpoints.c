#include "breakpoints.h"

void set_breakpoint(pid_t pid, void *addr)
{
    // Read original byte at addr
    uint8_t orig_byte = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);

    // Replace it with INT 3 (0xCC)
    uint8_t int3 = 0xCC;
    ptrace(PTRACE_POKEDATA, pid, addr, (orig_byte & 0xFFFFFF00) | int3);
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
        if (strstr(line, "r-xp") != NULL) {
            // Extract the base address from the first column
            sscanf(line, "%lx", &base_address);
            break;
        }
    }

    fclose(maps_file);
    return base_address;

}