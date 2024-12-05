#include "breakpoints.h"
#include <string.h>

ls_bp ls = {.count=0};

void set_breakpoint(pid_t pid, unsigned long addr)
{
    errno = 0;
    // Read original word at addr
    long orig_word = ptrace(PTRACE_PEEKDATA, pid, (void*)addr, NULL);
    printf("%ld\n", orig_word);
    if (orig_word == -1 && errno !=0)
    {
        perror("ptrace PEEKTEXT failed");
        return;
    }

    // Replace the least significant byte with INT 3 (0xCC)
    long int3_word = (orig_word & ~0xFF) | 0xCC;
    if (ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)int3_word) == -1)
    {
        perror("ptrace POKETEXT failed");
        return;
    }

    breakpoint bp = {.addr=addr, .original_data=orig_word,.is_enabled=true};
    ls.ls_bp[ls.count] = bp;
    ls.count++;
    
}

void remove_breakpoint(pid_t pid, unsigned long addr){
    for (int i = 0; i < ls.count; i++)
    {
        if(ls.ls_bp[i].addr == addr && ls.ls_bp[i].is_enabled == true)
        {
            if (ptrace(PTRACE_POKETEXT, pid, addr, (void *)ls.ls_bp[i].original_data) == -1) {
                perror("PTRACE_POKETEXT");
                return;
            }

            ls.ls_bp[i].is_enabled = false;
            printf("Breakpoint removed at %p\n", (void*)addr);
            return;
        }
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
       if (strstr(line, "r-xp")) {
            sscanf(line, "%lx", &base_address);
            break;
        }
    }

    fclose(maps_file);
    return base_address;

}