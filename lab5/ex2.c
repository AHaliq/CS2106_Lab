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
	if (!stream->flag_read || stream->flag_justwrote)
		return MY_EOF;
	// terminate if no read flag or didnt flush
	int remRead = nmemb * size;
	// calculate total bytes to read
	int dStart = 0;
	// capture start index for copy to destination
	int res;
	int totRead = 0;
	int readAmount;
	if (stream->cnt > 0)
	{
		readAmount = stream->cnt < remRead ? stream->cnt : remRead;
		memcpy(ptr + dStart, stream->ptr, readAmount);
		dStart += readAmount;
		stream->ptr += readAmount;
		stream->cnt -= readAmount;
		remRead -= readAmount;
		totRead += readAmount;
	}
	// read initial remaining in buffer
	if (remRead > 0)
	{
		stream->ptr = stream->base;
		stream->cnt = 0;

		int bufferAmount = remRead / BUFFER_SIZE;
		if (bufferAmount > 0)
		{
			res = read(stream->fd, ptr + dStart, bufferAmount * BUFFER_SIZE);
			if (res < 0)
				return MY_EOF;
			remRead -= res;
			totRead += res;
		}
		// read direct from if greater than buffer remaining
		res = read(stream->fd, stream->ptr, BUFFER_SIZE);
		if (res < 0)
		{
			return MY_EOF;
		}
		stream->cnt = res;
		readAmount = res < remRead ? res : remRead;
		memcpy(ptr + dStart, stream->ptr, readAmount);
		totRead += readAmount;
		stream->cnt -= readAmount;
		stream->ptr += readAmount;
		// read into buffer first if read is less than buffer size
	}
	return totRead;
}
