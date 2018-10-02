#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VERSION "charge-1.0.1"
#define INTERVAL 5
#define FORMAT "%s %i"
#define BAT_PATH "/sys/class/power_supply/BAT%i/uevent"
#define BAT_IND 0
#define STATUS_KEY "POWER_SUPPLY_STATUS"
#define CAPACITY_KEY "POWER_SUPPLY_CAPACITY"

int usage(const char *argv0, int ec);

int battery_output(char *path, char *format);

int
main(int argc, char **argv)
{
  char *path = BAT_PATH;
  char *format = FORMAT;
  int index = BAT_IND;
  int interval = INTERVAL;
  int stay = 0;

  int opt;
  while ((opt = getopt(argc, argv, "hvi:sf:n:p:")) != -1) {
    switch (opt) {
        case 'h':
            usage(argv[0], 0);
            break;
        case 'v':
            fprintf(stderr, "Version: %s\n", VERSION);
            exit(0);
            break;
        case 'i':
            interval = strtol(optarg, NULL, 10);
            break;
        case 's':
            stay = 1;
            break;
        case 'f':
            format = optarg;
            break;
        case 'n':
            index = strtol(optarg, NULL, 10);
            break;
        case 'p':
            path = optarg;
            break;
        default:
            usage(argv[0], 1);
            break;
    }
  }

  if ((setlocale(LC_ALL, "C")) == NULL) {
    perror("setlocale");
  }

  char real_path[256] = { 0 };
  snprintf(real_path, sizeof(real_path), path, index);

  if (stay) {
    while (battery_output(real_path, format) == 0) {
      sleep(interval);
    }
  } else {
    return battery_output(real_path, format);
  }

  return 0;
}

int
usage(const char *argv0, const int ec)
{
    fprintf(stderr,
            "Usage:\n"
            "    $ %s [[-s] [-i INTERVAL] [-f FORMAT] [-n INDEX] [-p PATH]] \n"
            "         -h          Show this help screen\n"
            "         -v          Show current version\n"
            "         -s          Stay for continuous output\n"
            "         -i          Interval for stay mode\n"
            "         -f          Format for output\n"
            "         -n          Index number of battery\n"
            "         -p          Path to power supply battery information\n"
            "\n",
            argv0);

    exit(ec);
}

int
battery_output(char *path, char *format)
{
    char line[256] = { 0 };
    char status[256] = { 0 };
    int capacity = -1;
    int found_status = 0;
    int found_capacity = 0;

    FILE *bf = fopen(path, "r");
    if (bf == NULL) {
        fprintf(stderr, "Path `%s' not recognised\n", path);
        return 1;
    }

    while (fgets(line, sizeof(line), bf) != NULL) {
        char *key = strtok(line, "=\n");
        if (key != NULL) {
            if (!found_status && strcmp(key, STATUS_KEY) == 0) {
                strncpy(status, strtok(NULL, "=\n"), sizeof(status));
                found_status = 1;
            } else if (capacity == -1 && strcmp(key, CAPACITY_KEY) == 0) {
                capacity = strtol(strtok(NULL, "=\n"), NULL, 10);
                found_capacity = 1;
            }
        }
    }
    fclose(bf);

    if (!found_capacity || !found_status) {
        fprintf(stderr, "Battery information missing\n");
        return 1;
    }

    if (strstr(format, "%s") == NULL) {
        fprintf(stdout, format, capacity);
    } else if (strstr(format, "%i") == NULL) {
        fprintf(stdout, format, status);
    } else {
        fprintf(stdout, format, status, capacity);
    }
    fprintf(stdout, "\n");
    fflush(stdout);

    return 0;
}
