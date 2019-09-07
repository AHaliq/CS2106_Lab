/*************************************
 * Lab 2 Exercise 2
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#define TOKEN_LEN 19
#define TOKEN_CNT 10 

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()
#include <string.h>     //for strcmp()
#include <stdarg.h>

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);

void print(char* str, ...);
void printError(char * str, ...);
char** readT(int* tokenCount);
int cfileexists(const char* filename);

const int INPUT_LENGTH = TOKEN_LEN + TOKEN_CNT * (1 + TOKEN_LEN) + 1;

int main() {
    print("Hello, welcome to Genie!\n");
    int tCount, childPid, run = 1;
    char** tokens;
    while(run) {
        tokens = readT(&tCount);

        if(strcmp(tokens[0], "quit") == 0) {
            run = 0;
        }else {
            char exists = 0;
            char fullPath[TOKEN_LEN + 5];
            sprintf(fullPath, "%s", tokens[0]);

            if(cfileexists(tokens[0])) {
                exists = 1;
            }else {
                sprintf(fullPath, "/bin/%s", tokens[0]);
                if(cfileexists(fullPath)) {
                    exists = 1;
                }else {
                    printError("%s not found\n", tokens[0]);
                }
            }
            // determine full path to command

            if(exists) {
                childPid = fork();
                if(childPid == 0) {
                    execv(fullPath, tokens);
                }else {
                    wait(NULL);
                }
            }
            // run child command
        }

        freeTokenArray(tokens, tCount);
    }
    print("Goodbye!\n");
}





char** readT(int* tokenCount) {
    print("> ");
    char buffer[INPUT_LENGTH];
    read(STDIN_FILENO, buffer, INPUT_LENGTH);
    return readTokens(TOKEN_CNT + 1, TOKEN_LEN, tokenCount, buffer);
    return NULL;
}

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* token;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);
    
    //Nullify all entries
    for (int i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    token = strtok(buffer, " \n");
    
    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], token, maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';
        
        i++;
        token = strtok(NULL, " \n");
    }
    
    *readTokenNum = i;
    
    return tokenStrArr;
}

void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

void print(char* str, ...) {
    char buffer[INPUT_LENGTH];
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buffer, sizeof buffer, str, argptr);
    va_end(argptr);

    write(STDOUT_FILENO, buffer, strlen(buffer));
}

void printError(char * str, ...) {
    char buffer[INPUT_LENGTH];
    va_list argptr;
    va_start(argptr, str);
    vsnprintf(buffer, sizeof buffer, str, argptr);
    va_end(argptr);

    write(STDERR_FILENO, buffer, strlen(buffer));
}

int cfileexists(const char* filename) {
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else // -1
        return 0;
}