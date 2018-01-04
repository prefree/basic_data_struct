#ifndef _ring_buffer_H_
#define _ring_buffer_H_

struct ring_buffer {
	unsigned char     *data;
	ssize_t           size;
	ssize_t           pread;
	ssize_t           pwrite;
};

/* initialize ring buffer*/
extern void ring_buffer_init(struct ring_buffer *rbuf, void *data, size_t len);

/* test whether buffer is empty */
extern int ring_buffer_empty(struct ring_buffer *rbuf);

/* return the number of free bytes in the buffer */
extern ssize_t ring_buffer_free(struct ring_buffer *rbuf);

/* return the number of bytes waiting in the buffer */
extern ssize_t ring_buffer_avail(struct ring_buffer *rbuf);

/* flush buffer */
extern void ring_buffer_flush(struct ring_buffer *rbuf);

/*
** read <len> bytes from ring buffer into <buf>
** returns number of bytes transferred or -EFAULT
*/
extern void ring_buffer_read(struct ring_buffer *rbuf,
				   unsigned char *buf, size_t len);


/*
** write <len> bytes to ring buffer
** returns number of bytes transferred or -EFAULT
*/
extern ssize_t ring_buffer_write(struct ring_buffer *rbuf,
			   	   const unsigned char *buf, size_t len);

#endif /* _ring_buffer_H_ */
