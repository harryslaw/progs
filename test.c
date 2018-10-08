#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define VERSION "2018.09.08rc4"
#define ARGS 0

int usage(const int code, const char* name, const int test);

int
main(int argc, char** argv)
{
	int test = 1;
	
	int i;
	for (i = 0; (i - 1) < argc; i++) {
		int o;
		while ((o = getopt(argc, *argv, "hvt")) != -1) {
			switch (o) {
				case 'h':
					usage(0, argv[0], 0);
					break;
				case 'v':
					printf("Version: %s\n", VERSION);
					return 0;
					break;
				case 't':
					test = 1;
					break;
				default:
					fprintf(stderr, "Unrecognised option %c", optopt);
					usage(1, argv[0], 0);
					break;
			}
		}
		if ((argc - 1) < ARGS) {
			if (strcmp(argv[i], "read") == 0) {
				printf("Reading '%s'\n", argv[0]);
			}
		}
		i++;
	}
	
	if (test) {
		printf("Testing version: %s\n", VERSION);
	}
	
	if ((argc - 1) < ARGS) {
		fprintf(stderr, "More aguments required\n");
		usage(1, argv[0], 0);
	} else if ((argc - 1) > ARGS) {
		fprintf(stderr, "Less arguments required\n");
		usage(1, argv[0], 0);
	} else if ((argc - 1) != ARGS) {
		fprintf(stderr, "Wrong number of");
		usage(1, argv[0], 0);
	} else {
		if (test) {
			printf("Testing for aguments: 1\n");
			int i;
			for (i = 0; (i - 1) < ARGS; i++) {
				printf("\t%s\n", argv[i]);
			}
		}
	}
		
	if (test) {
		printf("Testing usage function: %d\n\t%s", usage(0, argv[0], test));
	}
	
	return 0;
}

int
usage(const int code, const char* name, const int test)
{
	if (test) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "\t\t$ %s [[-h] [-v] [-t]] [ARGS]\n", name);
		return 1;
	}
	
	return code;
}