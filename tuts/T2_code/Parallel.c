#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int userInput, childPid, childResult;
    //Since largest number is 10 digits, a 12 characters string is more
    //than enough
    char cStringExample[12];
    

    while (scanf("%d", &userInput) == 1) {
        childPid = fork();
	if (childPid == 0) break;
    }

    if(childPid == 0) {
	//Easy way to convert a number into a string
	sprintf(cStringExample, "%d", userInput);
	childPid = fork();
	if (childPid == 0) {
	    execl("./PF", "PF", cStringExample, NULL);
	}
        waitpid(childPid, &childResult, 0);
        printf("%d has %d prime factors\n", userInput, childResult >> 8);
    }
    return 0;

}
