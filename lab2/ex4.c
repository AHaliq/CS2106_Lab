/*************************************
 * Lab 2 Exercise 4
 * Name: Abdul Haliq S/O Abdul Latiff
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#define TOKEN_LEN 19
#define TOKEN_CNT 10 
#define JOBS_MAX 10
#define ENV_MAX 10
#define _GNU_SOURCE

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

char*** readTokens(int maxJobs, int maxTokenNum, int maxTokenSize, int* readTokenNum, int* readJobNum, char* buffer);
void freeTokenArray(char*** strArr, int *size, int jsize);

void print(char* str, ...);
void printError(char * str, ...);
char*** readT(int* tokenCount, int* jobCount);
int cfileexists(const char* filename);
char** resolveEnvTokens(char** tokens, int tLen);

const int INPUT_LENGTH = TOKEN_LEN + TOKEN_CNT * (1 + TOKEN_LEN) + 1;
const int COMMAND_LENGTH = (TOKEN_LEN + TOKEN_CNT * (1 + TOKEN_LEN) + 3) * JOBS_MAX - 3 + 1;
const int ENV_LEN = TOKEN_LEN + 1 + TOKEN_LEN;

int main() {
    print("Hello, welcome to Genie!\n");
    int run = 1;

    while(run) {
        int tCount[JOBS_MAX];
        int jCount;
        char*** tokens = readT(tCount, &jCount);
        if(strcmp(tokens[0][0], "quit") == 0) {
            run = 0;
        } else if(strcmp(tokens[0][0], "set") == 0 && tCount[0] == 3) {
            setenv(tokens[0][1], tokens[0][2], 0);
        } else if(strcmp(tokens[0][0], "unset") == 0 && tCount[0] == 2) {
            unsetenv(tokens[0][1]);
        }else {
            char** fullPath = (char**) malloc(sizeof(char*) * jCount);
            for(int i = 0; i < jCount; i++) {
                fullPath[i] = (char*) malloc(sizeof(char) * (TOKEN_LEN + 5 + 1));
                sprintf(fullPath[i], "%s", tokens[i][0]);

                if(!cfileexists(tokens[i][0])) {
                    sprintf(fullPath[i], "/bin/%s", tokens[i][0]);
                    if(!cfileexists(fullPath[i])) {
                        printError("%s not found\n", tokens[i][0]);
                        break;
                    }
                }
                // check all commands exists
            }

            int totalFds = (jCount - 1) * 2;
            int fd[totalFds];
            for(int i = 0; i < totalFds; i+=2) pipe(fd + i);
            // setup pipes

            int j = 0;
            while(j < jCount) {
                if(fork() == 0) {
                    if(j > 0) dup2(fd[(j-1) * 2], 0);           // input from previous pipe
                    if(j < jCount - 1) dup2(fd[j * 2 + 1], 1);  // output to current pipe
                    for(int i = 0; i < totalFds; i++) close(fd[i]);

                    execvp(*tokens[j], resolveEnvTokens(tokens[j], tCount[j]));
                }
                j++;
            }
            for(int i = 0; i < totalFds; i++) close(fd[i]);
            // free file descriptors

            for(int i = 0; i < jCount; i++) wait(NULL);
            // free processes
        }
        freeTokenArray(tokens, tCount, jCount);
        // free tokens
    }
    
    print("Goodbye!\n");
    return 0;
}

char** resolveEnvTokens(char** tokens, int tLen) {
    char** resolved = (char**) malloc(sizeof(char*) * tLen + 1);
    resolved[tLen] = NULL;
    for(int i = 0; i < tLen; i++) {
        resolved[i] = tokens[i];
        if(tokens[i][0] == '$') {
            resolved[i] = getenv(tokens[i] + 1);
        }
    }
    return resolved;
}

char*** readT(int* tokenCount, int* jobsCount) {
    print("> ");
    char buffer[COMMAND_LENGTH];
    read(STDIN_FILENO, buffer, COMMAND_LENGTH);
    return readTokens(JOBS_MAX, TOKEN_CNT + 1, TOKEN_LEN, tokenCount, jobsCount, buffer);
    return NULL;
}


char*** readTokens(int maxJobs, int maxTokenNum, int maxTokenSize, int* readTokenNum, int* readJobNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char*** tokenStrArr;
    char *token, *job;
    char **jobs;
    int i, j, jCount = 0;

    for(int i = 0; i < strlen(buffer); i++) 
        if(buffer[i] == '\n') buffer[i] = '\0';

    jobs = (char**) malloc(sizeof(char*) * INPUT_LENGTH);
    for(int i = 0; i < maxJobs; i++) jobs[i] = NULL;

    // allocate token jobs array, each element is a char**
    tokenStrArr = (char***) malloc(sizeof(char**) * maxJobs);
    for (int i = 0; i < maxJobs; i++) tokenStrArr[i] = NULL;

    job = strtok(buffer, "|");    
    j = 0;
    while (j < maxJobs && (job != NULL)) {
        jobs[j] = (char*) malloc(sizeof(char*) * INPUT_LENGTH);
        strncpy(jobs[j], job, INPUT_LENGTH - 1);
        job = strtok(NULL, "|");
        j++;
    }
    jCount = j;
    *readJobNum = j;
    
    for(j = 0; j < jCount; j++) {
        //allocate token array, each element is a char*
        tokenStrArr[j] = (char**) malloc(sizeof(char*) * maxTokenNum);
        
        //Nullify all entries
        for (int i = 0; i < maxTokenNum; i++) {
            tokenStrArr[j][i] = NULL;
        }

        token = strtok(jobs[j], " \n");
        
        i = 0;
        while (i < maxTokenNum && (token != NULL)) {
            //Allocate space for token string
            tokenStrArr[j][i] = (char*) malloc(sizeof(char*) * maxTokenSize);

            //Ensure at most 19 + null characters are copied
            strncpy(tokenStrArr[j][i], token, maxTokenSize - 1);

            //Add NULL terminator in the worst case
            tokenStrArr[j][i][maxTokenSize-1] = '\0';
            
            i++;
            token = strtok(NULL, " \n");
        }
        readTokenNum[j] = i;
    }

    for(int i = 0; i < *readJobNum; i++) {
        free(jobs[i]);
    }
    free(jobs);
    
    return tokenStrArr;
}

void freeTokenArray(char*** tokenStrArr, int* size, int jsize) {
    int i = 0, j = 0;

    //Free every string stored in tokenStrArr
    for (j = 0; j < jsize; j++) {
        for (i = 0; i < size[j]; i++) {
            if (tokenStrArr[j][i] != NULL) {
                free(tokenStrArr[j][i]);
                tokenStrArr[j][i] = NULL;
            }
        }
        free(tokenStrArr[j]);
        tokenStrArr[j] = NULL;
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