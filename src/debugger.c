#include "debugger.h"
void debugger(pid_t p)
{
    int status;
    waitpid(p, &status, 0);
    // Continue the child process execution and handle signals.

    while (WIFSTOPPED(status)) 
    {
        // Continue the process after it stops
        ptrace(PTRACE_CONT, p, NULL, NULL);

        // Wait for the child process to stop again
        waitpid(p, &status, 0);

        // Check if the child has exited or terminated
        if (WIFEXITED(status) || WIFSIGNALED(status)) 
        {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
            break;
        }
    }

}

void run(char ** argv)
{
    // The child process.
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) { perror("ptrace"); exit(1);}
    kill(getpid(), SIGSTOP); // Stop the child process immediately
    execvp(argv[1], &argv[1]); // Execute the process passed as an argument
    perror("execvp"); // execvp only returns on error
    exit(1);
}

void printProgramAdresses(const char* binary_path)
{
    int fd = open(binary_path, O_RDONLY);
    if(fd < 0){
        perror("Failed to open binary file");
        exit(1);
    }

    //Read and parse the elf header.
    Elf64_Ehdr ehdr;
    if(read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)){
        perror("Failed to read elf header");
        close(fd);
        exit(1);
    }

    // look for lef header.
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not a valid ELF file\n");
        close(fd);
        exit(1);
    }
    //move the cursor to read from section header table.
    lseek(fd,ehdr.e_shoff, SEEK_SET);

    //try to read section header table.
    Elf64_Shdr* shdrs = malloc(ehdr.e_shnum * sizeof(Elf64_Shdr));
    if(read(fd,shdrs, ehdr.e_shnum * sizeof(Elf64_Shdr)) != ehdr.e_shnum * sizeof(Elf64_Shdr)){
        perror("Failed to read section header.");
        free(shdrs);
        close(fd);
        exit(1);
    }

    //Get string from the string table.
    Elf64_Shdr shstrtab = shdrs[ehdr.e_shstrndx];
    char* shstrtab_data = malloc(shstrtab.sh_size);
    lseek(fd, shstrtab.sh_offset, SEEK_SET);

    if (read(fd, shstrtab_data, shstrtab.sh_size) != shstrtab.sh_size) {
        perror("Failed to read section header string table");
        free(shdrs);
        free(shstrtab_data);
        close(fd);
        exit(1);
    }

    Elf64_Shdr* symtab_shdr = NULL;
    Elf64_Shdr* strtab_shdr = NULL;

    // look for the synbol table in all off the sections.
    for (int i = 0; i < ehdr.e_shnum ; i++)
    {
        const char* section_name = &shstrtab_data[shdrs[i].sh_name];

        if(shdrs[i].sh_type == SHT_SYMTAB)//if fount the symbol table we will save it.
        {
            symtab_shdr = &shdrs[i];
        }
        else if (strcmp(section_name, ".strtab") == 0)//if fount the string table we will save it.
        {
            strtab_shdr = &shdrs[i];
        }
    }

    // check if found header sections.
    if(!symtab_shdr || !strtab_shdr){
        fprintf(stderr, "Symbol table or string table not found\n");
        free(shdrs);
        free(shstrtab_data);
        close(fd);
        exit(1);
    }

    //Read the whole symbol table.
    Elf64_Sym* symtab = malloc(symtab_shdr->sh_size);
    lseek(fd, symtab_shdr->sh_offset, SEEK_SET);//move curse to symbl table offset in file.
    if(read(fd,symtab, symtab_shdr->sh_size) != symtab_shdr->sh_size)
    {
        perror("Failed to read symbol table");
        free(shdrs);
        free(shstrtab_data);
        free(symtab);
        close(fd);
        exit(1);
    }
    
    //Read the whole string table.
    char* strtab_data = malloc(strtab_shdr->sh_size);
    lseek(fd, strtab_shdr->sh_offset, SEEK_SET);
    if(read(fd, strtab_data, strtab_shdr->sh_size) != strtab_shdr->sh_size)
    {
        perror("Failed to read string table");
        free(shdrs);
        free(shstrtab_data);
        free(symtab);
        free(strtab_data);
        close(fd);
        exit(1);
    }


    //print symbol name and adresses.
    int num_sym = symtab_shdr->sh_size / sizeof(Elf64_Sym);
    for(int i = 0; i < num_sym; i++)
    {
        if(symtab[i].st_name !=0)
        {
            const char* symbol_name = &strtab_data[symtab[i].st_name];
            printf("0x%lx %s\n", symtab[i].st_value, symbol_name);
        }
    }
    
    free(shdrs);
    free(shstrtab_data);
    free(symtab);
    free(strtab_data);
    close(fd);
}

enum CMD getIdByCommand(char** cmd)
{
    if(strcmp(cmd[0],"s") == 0){
        return STEP;
    }
    if(strcmp(cmd[0],"c") == 0){
        return CON;
    }
    if(strcmp(cmd[0],"b") == 0){
        return BREAK;
    }
    if(strcmp(cmd[0],"regs") == 0){
        return REGS;
    }
    if(strcmp(cmd[0],"ins") == 0){
        return INS;
    }
    if(strcmp(cmd[0],"quit") == 0){
        return QUIT;
    }
}


void doCommand(char* fullcmd)
{
    char** cmd = parsecmd(fullcmd);
    enum CMD com = getIdByCommand(cmd);
    if(com == QUIT)
    {
        freeCmd(cmd);
        exit(1);
    };
}

int checkIfFileExists(const char* fname){
    FILE* file;
    if((file = fopen(fname, "r"))){
        fclose(file);
        return 1;
    }
    return 0;
}