#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION "array-1.0.0"
#define SIZE 10
#define MAX 50

static int check_number(const char *num);

int main(int argc, char **argv) {
  int arraySize = SIZE;
  int maxNumber = MAX;
  int randArray[arraySize];

  int opt;
  while ((opt = getopt(argc, argv, "hvs:n:")) != -1) {
    switch (opt) {
    case 'h':
      fprintf(stderr, "%s [-h] [-v] [-s SIZE] [-n NUM]\n", argv[0]);
      exit(0);
      break;
    case 'v':
      fprintf(stderr, "Version: %s\n", VERSION);
      exit(0);
      break;
    case 's':
      if (check_number(optarg) == 0) {
        arraySize = strtol(optarg, NULL, 10);
      } else {
        fprintf(stderr, "A number is required\n");
      }
      break;
    case 'n':
      if (check_number(optarg) == 0) {
        maxNumber = strtol(optarg, NULL, 10);
      } else {
        fprintf(stderr, "A number is required\n");
      }
      break;
    default:
      fprintf(stderr, "%s [-h] [-v] [-s SIZE] [-n NUM]\n", argv[0]);
      break;
    }
  }

  for (int i = 0; i < arraySize; i++) {
    srand(clock());
    randArray[i] = rand() % (maxNumber - 1);
    fprintf(stderr, "%d ", randArray[i]);
  }

  fprintf(stderr, "\n");

  return 0;
}

static int check_number(const char *num) {
  if (num[0] == '-') {
    fprintf(stderr, "Negative numbers cannot be used\n");
  }
  for (int i = 0; num[i] != 0; i++) {
    if (!isdigit(num[i])) {
      return 1;
    }
  }
  return 0;
}
