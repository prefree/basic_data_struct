#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "ring_buffer.h"

#define RING_BUFFER_SIZE     20
#define INPUT_FILE           "input.cap"
#define OUTPUT_FILE          "output.cap"
struct rtest {
	int      running;
	struct   ring_buffer rbuf;
	FILE     *input_fp;
	FILE     *output_fp;
};

int get_delay_time()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srandom(tv.tv_sec*1000+tv.tv_usec/1000);
	return random()%10;
}

void * read_func(void *arg)
{
	struct rtest *ctx;
	unsigned char buf[1024];
	int avail = 0, len = 0;

	ctx = (struct rtest*)arg;
	ctx->output_fp = fopen(OUTPUT_FILE, "w+");
	if (!ctx->output_fp) {
		printf("%s, %s -> %s\n", __func__, OUTPUT_FILE, strerror(errno));
		return NULL;
	}
	memset(buf, 0, sizeof(buf));
	do {
		avail = ring_buffer_avail(&ctx->rbuf);
		if (avail) {
			ring_buffer_read(&ctx->rbuf, buf, avail);
			len = fwrite(buf, 1, avail, ctx->output_fp);
			printf("read -> %s, len->%d\n", buf, len);
		}
		usleep(get_delay_time()*1000);
	} while (ctx->running);

}

void * write_func(void *arg)
{
	struct rtest *ctx;
	int free = 0, len = 0;
	char buf[32*1024] = {0};

	ctx = (struct rtest*)arg;
	ctx->input_fp = fopen(INPUT_FILE, "r");
	if (!ctx->input_fp) {
		printf("%s, %s -> %s\n", __func__, INPUT_FILE, strerror(errno));
		return NULL;
	}
	do {
		free = ring_buffer_free(&ctx->rbuf);
		len = fread(buf, 1, free, ctx->input_fp);
		if (len) {
		    ring_buffer_write(&ctx->rbuf, buf, len);
		    printf("write -> %d \n", len);
		}
		if (feof(ctx->input_fp)) {
			printf("%s read eof\n", INPUT_FILE);
			break;
		}
		usleep(get_delay_time()*1000);
	} while (ctx->running);
}
void create_input_file()
{
	char cmd[512];

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "cat /proc/cpuinfo > %s", INPUT_FILE);
	system(cmd);
}
int main(int argc, char **argv)
{
	pthread_t rt, wt;
	int ret;
	unsigned char *buf;
	unsigned char fbuf[256];
    struct rtest ctx;

	ctx.running = 1;
	create_input_file();
	if (access(INPUT_FILE, R_OK)) {
		printf("%s -> %s\n", INPUT_FILE, strerror(errno));
		return 1;
	}
	buf = malloc(RING_BUFFER_SIZE);
	if (!buf) {
		printf("malloc failed\n");
		return 1;
	}
	ring_buffer_init(&ctx.rbuf, buf, RING_BUFFER_SIZE);
	ret = pthread_create(&rt, NULL, read_func, (void *)&ctx);
	if (ret) {
		printf("read thread create failed: %s\n", strerror(errno));
		return 1;
	}
	ret = pthread_create(&wt, NULL, write_func, (void *)&ctx);
	if (ret) {
		printf("write thread create failed: %s\n", strerror(errno));
		return 1;
	}

	for (;;) {
		if (fgets(fbuf, 256, stdin)) {
			if (!strncmp(fbuf, "quit", 4)) {
				ctx.running = 0;
				printf("quit app\n");
				break;
			}
		}
	}

	pthread_join(rt, NULL);
	pthread_join(wt, NULL);
    ring_buffer_flush(&ctx.rbuf);
	if (ctx.rbuf.data)
		free(ctx.rbuf.data);
	return 0;
}
