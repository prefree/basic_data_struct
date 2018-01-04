#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "ring_buffer.h"

#define RING_BUFFER_SIZE 20
#define TEXT_LEN 1000
#define R_DELAY 500
#define W_DELAY 1000
int running = 1;
char text[TEXT_LEN];
void * read_func(void *arg)
{
	struct ring_buffer *rbuf;
	unsigned char buf[1024];
	int avail = 0;

	rbuf = (struct ring_buffer *)arg;
	memset(buf, 0, sizeof(buf));
	do {
		avail = ring_buffer_avail(rbuf);
		if (avail) {
			ring_buffer_read(rbuf, buf, avail);
			printf("read -> %s\n", buf);
			memset(buf, 0, sizeof(buf));
		}
		usleep(R_DELAY*1000);
	} while (running);

}

void * write_func(void *arg)
{
	struct ring_buffer *rbuf;
	int free = 0, len = 0, left = TEXT_LEN;

	rbuf = (struct ring_buffer *)arg;
	do {
		free = ring_buffer_free(rbuf);
		len = (left < free ? left : free);
		ring_buffer_write(rbuf, text+TEXT_LEN-left, len);
		left -= len;
		if (!left) {
			left = TEXT_LEN;
		} else if (left < 0) {
			printf("write error!\n");
			break;
		}
		sleep(W_DELAY*1000);
	} while (running);
}
void text_fill()
{
	int i;
	for (i=0; i<TEXT_LEN; i+=10) {
		memcpy(text+i, "0123456789", 10);
	}
}
int main(int argc, char **argv)
{
	pthread_t rt, wt;
	struct ring_buffer rbuf;
	int ret;
	unsigned char *buf;
	unsigned char fbuf[256];

	text_fill();
	buf = malloc(RING_BUFFER_SIZE);
	if (!buf) {
		printf("malloc failed\n");
		return 1;
	}
	ring_buffer_init(&rbuf, buf, RING_BUFFER_SIZE);
	ret = pthread_create(&rt, NULL, read_func, (void *)&rbuf);
	if (ret) {
		printf("read thread create failed: %s\n", strerror(errno));
		return 1;
	}
	ret = pthread_create(&wt, NULL, write_func, (void *)&rbuf);
	if (ret) {
		printf("write thread create failed: %s\n", strerror(errno));
		return 1;
	}

	for (;;) {
		if (fgets(fbuf, 256, stdin)) {
			if (!strncmp(fbuf, "quit", 4)) {
				running = 0;
				printf("quit app\n");
				break;
			}
		}
	}

	pthread_join(rt, NULL);
	pthread_join(wt, NULL);
    ring_buffer_flush(&rbuf);
	if (rbuf.data)
		free(rbuf.data);
	return 0;
}
