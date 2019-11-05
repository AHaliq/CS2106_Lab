/*************************************
 * Lab 5 Ex3
 * Name: Abdul Haliq S/O Abdul Latiff
 * Student No: A0125431U
 * Lab Group:B-08
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream)
{
	if (stream->cnt > 0 || !stream->flag_write)
	{
		return MY_EOF;
	}
	int res;
	// if read just before or no write flag
	int remWrite = nmemb * size;

	int bufSpace = stream->base + BUFFER_SIZE - stream->ptr;
	int writeAmount = remWrite < bufSpace ? remWrite : bufSpace;
	int sStart = 0;
	if (writeAmount > bufSpace)
	{
		memcpy(stream->ptr, ptr, writeAmount);
		stream->ptr += writeAmount;
		sStart += writeAmount;
		remWrite -= writeAmount;
		// write remaining available space to buffer

		res = my_fflush(stream);
		if (res < 0)
			return MY_EOF;
		// flush full buffer

		int bufferFlushes = remWrite / BUFFER_SIZE;
		if (bufferFlushes > 0)
		{
			res = write(stream->fd, ptr + sStart, BUFFER_SIZE * bufferFlushes);
			if (res < 0)
				return MY_EOF;
			remWrite -= BUFFER_SIZE * bufferFlushes;
			sStart += BUFFER_SIZE * bufferFlushes;
		}
		// if need more flushes collect all in one write

		writeAmount = remWrite;
	}

	memcpy(stream->ptr, ptr + sStart, writeAmount);
	stream->ptr += writeAmount;
	// store remaining in buffer

	stream->flag_justwrote = 1;

	return nmemb;
}
