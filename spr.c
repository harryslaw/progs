#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#define VERSION "spr-1.0.0"

int usage(const char *argv0, const int ec);

int file_bytes(FILE *fp, const char *path);

int
main(int argc, char **argv)
{
    FILE *fp = NULL;

    int opt;
    for (int ind = optind; ind < argc; ind++) {
        if ((opt = getopt(argc, argv, "hvf:")) != -1) {
            switch (opt) {
                case 'h':
                    usage(argv[0], 0);
                    break;
                case 'v':
                    fprintf(stderr, "Version: %s\n", VERSION);
                    exit(0);
                    break;
                default:
                    usage(argv[0], 1);
                    break;
            }
        } else {
            int fb;
            if ((fb = file_bytes(fp, argv[ind])) != 0) {
                exit(fb);
            }
        }
    }

    return 0;
}

int
file_bytes(FILE *fp, const char *path)
{
    char *buf;
    long len;

    if ((fp = fopen(path, "rb")) == NULL) {
        fprintf(stderr, "Error opening `%s'\n", path);
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    buf = (char *)malloc((len + 1) * sizeof(char));
    fread(buf, len, 1, fp);
    fclose(fp);

    fprintf(stdout, "%s", buf);
    fflush(stdout);
    free(buf);

    return 0;
}

int
usage(const char *argv0, const int ec)
{
    fprintf(stderr,
            "Usage:\n"
            "    $ %s [[-h] [-v] FILE [FILE ...]] \n"
            "         -h          Show this help screen\n"
            "         -v          Show current version\n"
            "\n",
            argv0);

    exit(ec);
}
