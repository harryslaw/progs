#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VERSION "20181008rc2"
#define ARGS 0

int usage(const int code, const char* name);

char* file_read(const char* path);

int
main(int argc, char** argv)
{
	int verbose = 1;
	char* read = NULL;
	
    --argc;
	if (argc < ARGS) {
		perror("arguments");
		usage(1, argv[0]);
	} else if (argc > ARGS) {
		perror("arguments");
		usage(1, argv[0]);
	} else if (argc != ARGS) {
		perror("arguments");
		usage(1, argv[0]);
	}
	
	int i;
	for (i = 0; i < argc; i++) {
		int o;
		while ((o = getopt(argc, argv, "hVv")) != -1) {
			switch (o) {
				case 'h':
					usage(0, argv[0]);
					break;
				case 'V':
					printf("Version: %s\n", VERSION);
					return 0;
					break;
				case 'v':
					verbose = 1;
					break;
				default:
					perror("unrecognised option");
					usage(1, argv[0]);
					break;
			}
		}
	
		if (strcmp(argv[i], "read") == 0) {
    int j;
    for (j = 0; j <= argc; j++) {
				read = file_read(argv[i + 1]);
				printf("File: '%s'\n", argv[i + 1]);
				printf(read);
			} 
		}
		
		i++;
	}
	
	fflush(stdout);
	return 0;
}

int
usage(const int code, const char* name)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "\t$ %s [[-h] [-V] [-v]] [ARGS]\n", name);

	return code;
}

int
file_read(const char* path)
{
	FILE* fs;
	long len;
	char* buf = NULL;
	
	if ((fs = fopen(path, "rb")) == NULL) {
		perror("file");
		return 1;
	}
	
	fseek(fs, 0, SEEK_END);
	len = ftell(fs);
	rewind(fs);
	
	buf = (char* )malloc((len + 1) * sizeof(char));
	fread(buf, len, 1, fs);
	buf[len] = '\0';
	
	fclose(fs);
        printf(buf);
	return 0;
}

