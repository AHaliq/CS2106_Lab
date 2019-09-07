/*************************************
 * Lab 2 Exercise 1
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

void readFromFile(char who[50], int fd);

int main(int argc, char **argv)
{
    char *fileName = NULL;
    int nChild = 0;
    int fd = 0;
    char who[50];

    int childPid, i;
    char strBuf[128];

    if (argc < 3)
    {
        printf("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }

    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open(fileName, O_RDONLY);

    if (fd != -1)
    {
        for (i = 0; i < nChild; i++)
        {
            childPid = fork();
            if (childPid == 0)
                break;
        }
        if (childPid == 0)
        {
            sprintf(who, "Child %d[%d]", i + 1, getpid());
        }
        else
        {
            sprintf(who, "Parent [%d]", getpid());
        }
        // get process title and pid

        readFromFile(who, fd);

        if (childPid != 0)
        {
            int status;
            for (i = 0; i < nChild; i++)
            {
                childPid = wait(&status);
                sprintf(strBuf, "Parent: Child %d [%d] done.\n", WEXITSTATUS(status), childPid);
                write(STDOUT_FILENO, strBuf, strlen(strBuf));
            }
            close(fd);
        }
        // parent waits for all children
    }
    
    if(childPid != 0) {
        sprintf(strBuf, "Parent: Exiting.\n");
        write(STDOUT_FILENO, strBuf, strlen(strBuf));
    }
    return i;
}

void readFromFile(char who[50], int fd)
{
    ssize_t readBytes = 1;
    char charBuf = 0;
    char strBuf[128];

    while (readBytes > 0)
    {
        usleep(1000);
        charBuf = 0;
        readBytes = read(fd, &charBuf, 1);

        if (readBytes != 1)
        {
            if (readBytes == 0)
            {
                //printf("EOF\n");
                return;
            }
        }
        sprintf(strBuf, "%s: %c\n", who, charBuf);
        write(STDOUT_FILENO, strBuf, strlen(strBuf));
        //TODO add your code
    }
}