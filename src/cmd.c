#include "cmd.h"



char** parsecmd(char* command)
{
    const char* del = " ";
    char* result[MAX_FLAGS] = (char**)malloc(MAX_FLAGS * sizeof(char*));
    int count = 0;

    if(result == NULL){
        perror("parseCmmd: problem allocating result\n");
        return NULL;
    }

    //taking the first token to start parsing.
    char* token  = strtok(command, del);
    int i = 0;
    while(token != NULL && i < MAX_FLAGS)
    {
        //allocate space for token.
        result[i] = (char*)malloc(strlen(token) + 1);
        if(result[i] == NULL)
        {
            perror("Problem allocating memory to cmd, free rest.\n");
            for (int j = 0; result[j] != NULL; j++)
            {
                free(result[j]);
            }
            free(result);
            return NULL;
        }
        //copy the the token to array.
        strncpy(result[i], token, strlen(result[i]));
        i++;
        token = strtok(NULL, del);
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