/*************************************
 * Lab 5 Ex2
 * Name: Abdul Haliq S/O Abdul Latiff
 * Student No: A0125431U
 * Lab Group: B-08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fread(void *ptr, size_t size, size_t nmemb, MY_FILE *stream)
{
	int remRead = nmemb * size;
	// calculate total bytes to read
	int dStart = 0;
	// capture start index for copy to destination
	do
	{
		if (stream->cnt == 0)
		{
			stream->ptr = stream->base;
			int res = read(stream->fd, stream->base, BUFFER_SIZE);
			if (res == -1)
				return -1;
			// error reading file
			if (res == 0)
				break;
			// if read none, no more, escape loop
			stream->cnt = res;
		}
		// read new set to buffer if no more available for read
		int copyAmount = stream->cnt > remRead ? remRead : stream->cnt;
		// determine how many bytes to copy from buffer
		memcpy(ptr + dStart, stream->ptr, copyAmount);
		// copy from buffer to destination
		dStart += copyAmount;
		stream->cnt -= copyAmount;
		stream->ptr += copyAmount;
		remRead -= copyAmount;
		// update variables
	} while (remRead > 0);
	int bytesRead = (nmemb * size) - remRead;
	int partialItem = bytesRead % size > 0 ? 1 : 0;
	return bytesRead == 0 ? 0 : bytesRead / size + partialItem;
}