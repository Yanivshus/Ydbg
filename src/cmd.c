#include "cmd.h"

char** parsecmd(char* command)
{
    char** result = (char**)malloc(MAX_FLAGS * sizeof(char));
    
    if(result == NULL){
        perror("parseCmd: problem allocating result\n");
        return NULL;
    }
    
    //taking the first token to start parsing.
    char* token = strtok(command, " ");
    int i = 0;
    while(token != NULL && i < MAX_FLAGS)
    {
        //allocate space for token.
        result[i] = (char*)malloc(strlen(token) + 1);
        if(result[i] == NULL)
        {
            perror("Problem allocating memory to cmd, free rest.\n");
            for (int j = 0; j < i; j++)
            {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        //copy the the token to array.
        strcpy(result[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    result[i] = NULL;
    return result;
}

void freeCmd(char** command){
    for (int i = 0; command[i] != NULL; i++)
    {
        free(command[i]);
    }
    free(command);
    
}