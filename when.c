#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define VERSION "when-1.0.4"
#define MAX_LEN 256
#define INTERVAL 5
#define FORMAT "%a %H:%M"

int usage(int ec);

int output(char *format);

int
main(int argc, char **argv)
{
  int stay = 0;
  int interval = INTERVAL;
  char *format = FORMAT;

  int opt;
  while ((opt = getopt(argc, argv, "hvi:sf:")) != -1) {
    switch (opt) {
    case 's':
      stay = 1;
      break;
    case 'i':
      interval = strtol(optarg, NULL, 10);
      break;
    case 'f':
      format = optarg;
      break;
    case 'h':
      usage(0);
      break;
    case 'v':
      fprintf(stderr, "Version: %s\n", VERSION);
      exit(0);
      break;

    default:
      usage(1);
      break;
    }
  }

  if ((setlocale(LC_ALL, "C")) == NULL) {
    perror("setlocale");
  }

  if (stay) {
    while (output(format) == 0) {
      sleep(interval);
    }
  } else {
    return output(format);
  }

  return 0;
}

int
usage(int ec)
{
  fprintf(
      stderr,
      "Usage: $ when [[-h] [-v] [-i INTERVAL] [-s] [-f FORMAT]]\n"
      "        -h        Show this help screen\n"
      "        -v        Show current version number\n"
      "        -i        Set interval for continous mode\n"
      "        -s        Set continous output mode\n"
      "        -f        Format of output string'\n");
  exit(ec);
}

int output(char *format)
{
  char buf[MAX_LEN];
  time_t ep;
  struct tm *lt;

  if ((ep = time(NULL)) != -1 && (lt = localtime(&ep)) != NULL) {
     if (strftime(buf, sizeof(buf), format, lt) > 0 && \
          printf("%s\n", buf) > 0 && \
          fflush(stdout) != EOF) {
              return 0;
          }
  }

  return 1;
}
