#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <openssl/sha.h>

#define VERSION "hash-1.0.0"

int usage(const char *argv0, const int ec);

char *get_file(const char *path);

char *sha256_file(const char *path, char out[65]);

int
main(int argc, char **argv)
{
    int cat = 0;
    int show = 0;
    char sh[65];
    char *output;

    int opt;
    for (int ind = optind; ind < argc; ind++) {
        if ((opt = getopt(argc, argv, "hvsc")) != -1) {
            switch (opt) {
                case 'h':
                    usage(argv[0], 0);
                    break;
                case 'v':
                    fprintf(stderr, "Version: %s\n", VERSION);
                    exit(0);
                    break;
                case 'c':
                    cat = 1;
                    break;
                case 's':
                    show = 1;
                    break;
                default:
                    usage(argv[0], 1);
                    break;
            }
        } else {
            if (cat) {
                output = get_file(argv[ind]);
                fprintf(stdout, "%s", output);
            }
            if (show) {
                output = sha256_file(argv[ind], sh);
                fprintf(stdout, "%s  %s\n", output, argv[ind]);
            }
        }
    }
    fflush(stdout);

    return 0;
}

char *
get_file(const char *path)
{
    FILE *fp;
    char *buf = NULL;
    long len;

    if ((fp = fopen(path, "rb")) == NULL) {
        fprintf(stderr, "Error opening `%s'\n", path);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    rewind(fp);

    buf = (char *)malloc((len + 1) * sizeof(char));
    fread(buf, len, 1, fp);
    fclose(fp);

    buf[len] = '\0';

    return buf;
}

char *
sha256_file(const char *path, char out[65])
{
    FILE *fp;
    unsigned char sha[SHA256_DIGEST_LENGTH];
    unsigned char *buf;
    int br = 0;
    int bs = 32768;
    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    if ((fp = fopen(path, "rb")) == NULL) {
        fprintf(stderr, "Error opening `%s'\n", path);
        exit(1);
    }

    buf = (unsigned char *)malloc(bs * sizeof(char));
    while ((br = fread(buf, 1, bs, fp))) {
        SHA256_Update(&ctx, buf, br);
    }
    SHA256_Final(sha, &ctx);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(out + (i * 2), "%02x", sha[i]);
    }
    out[64] = 0;

    fclose(fp);
    free(buf);

    return out;
}

int
usage(const char *argv0, const int ec)
{
    fprintf(stderr,
            "Usage:\n"
            "    $ %s [[-h] [-v] [-c] [-s] FILE [FILE ...]] \n"
            "         -h          Show this help screen\n"
            "         -v          Show current version\n"
            "         -c          Print the file(s) contents\n"
            "         -s          Check file(s) hash (SHA256)\n"
            "\n",
            argv0);

    exit(ec);
}
