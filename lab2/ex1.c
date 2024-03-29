/*************************************
 * Lab 2 Exercise 1
 * Name: Abdul Haliq S/O Abdul Latiff
 * Student No: A0125431U
 * Lab Group: B-08
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

void readFromFile(char who[50], int fd, int r1);
void readFromFileF(char who[50], FILE* fp, int r1);

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
        sprintf(who, "Parent [%d]", getpid());
	readFromFile(who, fd, 1);
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
        // get process title and pid

        readFromFile(who, fd, 0);

        if (childPid != 0)
        {
            int status;
            for (i = 0; i < nChild; i++)
            {
                childPid = wait(&status);
                sprintf(strBuf, "Parent: Child %d [%d] done.\n", WEXITSTATUS(status), childPid);
                write(STDOUT_FILENO, strBuf, strlen(strBuf));
            }
        }
        // parent waits for all children
        close(fd);
    }
    
    if(childPid != 0) {
	FILE* fp = fopen(fileName, "r");
	if(fp != NULL) {
	    readFromFileF(who, fp, 1);
	    for(i = 0; i < nChild; i++) {
		childPid = fork();
		if(childPid == 0) break;
	    }
 	    if(childPid == 0) sprintf(who, "Child %d[%d]", i + 1, getpid());
	
	    readFromFileF(who, fp, 0);
	    if(childPid != 0) {
		int status;
		for(i = 0; i < nChild; i++) {
			childPid = wait(&status);
			sprintf(strBuf, "Parent: Child %d [%d] done.\n", WEXITSTATUS(status), childPid);
			write(STDOUT_FILENO, strBuf, strlen(strBuf));
		}
	    }
	    fclose(fp);
	}
	if(childPid != 0) {
        sprintf(strBuf, "Parent: Exiting.\n");
        write(STDOUT_FILENO, strBuf, strlen(strBuf));
	}
    }
    return i + 1;
}

void readFromFileF(char who[50], FILE* fp, int r1) {
    ssize_t readBytes = 1;
    char charBuf = 0;
    char strBuf[128];
    
    while(readBytes > 0) {
	usleep(1000);
        charBuf = 0;
        readBytes = fread(&charBuf, 1, 1, fp);
	
	if (readBytes != 1) if(readBytes == 0) return;
	sprintf(strBuf, "%s: %c\n", who, charBuf);
	write(STDOUT_FILENO, strBuf, strlen(strBuf));
	if(r1) return;
    }
}

void readFromFile(char who[50], int fd, int r1)
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
	if(r1) return;
    }
}
