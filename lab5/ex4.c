/*************************************
 * Lab 5 Ex4
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream)
{
	int ret;
	if (stream->flag_write)
	{
		ret = write(stream->fd, stream->base, stream->ptr - stream->base);
	}
	stream->ptr = stream->base;
	stream->flag_justwrote = 0;
	return ret >= 0 ? 0 : MY_EOF;
}

int my_fseek(MY_FILE *stream, long offset, int whence)
{
	int res;
	if (stream->flag_justwrote && stream->flag_write)
	{
		res = my_fflush(stream);
		if (res < 0)
			return MY_EOF;
	}
	stream->ptr = stream->base;
	stream->cnt = 0;
	res = lseek(stream->fd, offset, whence);
	if (res < 0)
		return MY_EOF;

	return res;
}