/*************************************
 * Lab 5 my_stdio.h
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

/*
 * CS2106 my_stdio Library
 */

#ifndef MY_STDIO_H
#define MY_STDIO_H

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define MY_EOF (-1)
#define BUFFER_SIZE 4096

typedef struct
{
	unsigned int fd;		 // file descriptor
	unsigned char *base; // the buffer
	unsigned char *ptr;	// current position
	ssize_t cnt;				 // number of available characters in buffer
	char flag_read;
	char flag_write;
	char flag_justwrote;
} MY_FILE;

MY_FILE *my_fopen(const char *pathname, const char *mode);
int my_fclose(MY_FILE *stream);

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream);
size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream);
int my_fflush(MY_FILE *stream);
int my_fseek(MY_FILE *stream, long offset, int whence);

#endif /* MY_STDIO_H */
