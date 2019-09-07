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
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readFromFile (char who[50], int fd);

int main(int argc, char **argv) {
    char* fileName = NULL;
    int nChild = 0;
    int fd = 0;
    char who[50];
    
    if (argc < 3)
    {
        printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }
    
    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open( fileName, O_RDONLY );
    //TODO check the error code
    //TODO add your code

    sprintf(who, "Parent [%d]", getpid());

    readFromFile(who, fd );
    close (fd);

    //TODO add your code
    
}   

void readFromFile (char who[50], int fd) 
{
	ssize_t readBytes = 1;
	char charBuf = 0;
    char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = read( fd, &charBuf, 1 );

		if( readBytes != 1 ) 
        {
			if( readBytes == 0 ) 
            {
				printf( "EOF\n");
				return;
			}
		}
        sprintf(strBuf, "%s: %c\n", who, charBuf);
	    write (STDOUT_FILENO, strBuf, strlen(strBuf));
		//TODO add your code
	}
}
