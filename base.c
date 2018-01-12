#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

int debug_level = 0;

static void usage()
{
	printf("Usage: test [-h] [-d <debug>] [-f <filename>]\n");
	printf("\n");
	printf(" -d | --debug          : debug level (default:none, error, warn, debug)\n");
    printf(" -h | --help           : help information\n");
    printf(" -f | --file           : filename\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	char c;
	char *input = NULL;

	if (argc == 1)
		usage();
	static const struct option long_options [] = {
		{ "debug",        required_argument, NULL, 'd' },      
		{ "help",         no_argument,       NULL, 'h' },
		{ "file",         required_argument, NULL, 'f' },
		{ NULL, 0, NULL, 0 }
	};
	while ((c = getopt_long(argc, argv, "d:f:h", long_options, NULL)) != -1) {
		switch (c) {
			case 'd':
				if (optarg) {
					debug_level = 0;
					if (!strncmp(optarg, "error", 5)) {
						debug_level = 1;
					} else if (!strncmp(optarg, "warn", 4)) {
						debug_level = 2;
					} else if (!strncmp(optarg, "debug", 5)) {
						debug_level = 3;
					}
					printf("set debug level -> %d\n", debug_level);
				}
				break;

			case 'f':
				if (optarg) {
					if (asprintf(&input, "%s", optarg) < 0) {
						fprintf(stderr, "arguments error:%s, %d\n", 
								strerror(errno), __LINE__);
						exit(EXIT_FAILURE);
					}
					printf("input file -> %s\n", input);
				}
				break;

			case ':':
				fprintf(stderr, "option %c is missing arguments\n", c);
				exit(EXIT_FAILURE);
				break;

			case '?':
				fprintf(stderr, "unknow option %c\n", c);
				exit(EXIT_FAILURE);
				break;

			case 'h':
			default:
				usage();
				break;

		}
	}

	free(input);

	return 0;
}
