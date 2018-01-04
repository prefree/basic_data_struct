#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ring_buffer.h"

void ring_buffer_init(struct ring_buffer *rbuf, void *data, size_t len)
{
	rbuf->pread=rbuf->pwrite=0;
	rbuf->data=data;
	rbuf->size=len;
}

int ring_buffer_empty(struct ring_buffer *rbuf)
{
	return (rbuf->pread==rbuf->pwrite);
}

ssize_t ring_buffer_free(struct ring_buffer *rbuf)
{
	ssize_t free;

	free = rbuf->pread - rbuf->pwrite;
	if (free <= 0)
		free += rbuf->size;
	return free-1;
}

ssize_t ring_buffer_avail(struct ring_buffer *rbuf)
{
	ssize_t avail;

	avail = rbuf->pwrite - rbuf->pread;
	if (avail < 0)
		avail += rbuf->size;
	return avail;
}

void ring_buffer_flush(struct ring_buffer *rbuf)
{
	rbuf->pread = rbuf->pwrite;
}

void ring_buffer_read(struct ring_buffer *rbuf, unsigned char *buf, size_t len)
{
	size_t todo = len;
	size_t split;

	split = (rbuf->pread + len > rbuf->size) ? rbuf->size - rbuf->pread : 0;
	if (split > 0) {
		memcpy(buf, rbuf->data+rbuf->pread, split);
		buf += split;
		todo -= split;
		rbuf->pread = 0;
	}
	memcpy(buf, rbuf->data+rbuf->pread, todo);

	rbuf->pread = (rbuf->pread + todo) % rbuf->size;
}

ssize_t ring_buffer_write(struct ring_buffer *rbuf, 
				const unsigned char *buf, size_t len)
{
	size_t todo = len;
	size_t split;

	split = (rbuf->pwrite + len > rbuf->size) ? rbuf->size - rbuf->pwrite : 0;

	if (split > 0) {
		memcpy(rbuf->data+rbuf->pwrite, buf, split);
		buf += split;
		todo -= split;
		rbuf->pwrite = 0;
	}
	memcpy(rbuf->data+rbuf->pwrite, buf, todo);
	rbuf->pwrite = (rbuf->pwrite + todo) % rbuf->size;

	return len;
}
