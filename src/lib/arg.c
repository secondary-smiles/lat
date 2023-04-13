#include "arg.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LAT_USAGE "usage: lat [-cnVh] [files..]"

void help(void) {
  printf("%s\n", LAT_USAGE);
  printf("options:\n"
         "\t-c, --color\t toggle whether to print color or not\n"
         "\t-n, --lines\t toggle whether to print line numbers or not\n"
         "\t-V, --version\t show program version\n"
         "\t-h, --help\t display this help text\n");
}

void version(void) {
  printf("lat - v%s | %s:%s\n", LAT_VERSION, __DATE__, __TIME__);
}

struct config conf;
void argerr(char *r, char *arg) {
  printf("lat: %s '%s'\n", r, arg);
  printf("%s\n", LAT_USAGE);
  exit(EXIT_FAILURE);
}

void parselongarg(char *arg) {
  if (strcmp(arg, "--color") == 0) {
    conf.color = !conf.color;
    return;
  }

  if (strcmp(arg, "--lines") == 0) {
    conf.lines = !conf.lines;
    return;
  }

  if (strcmp(arg, "--help") == 0) {
    help();
    exit(EXIT_SUCCESS);
    return;
  }

  if (strcmp(arg, "--version") == 0) {
    version();
    exit(EXIT_SUCCESS);
    return;
  }

  argerr("unrecognized arg", arg);
}

void parseshortarg(char *arg) {
  size_t i = 1;
  while (arg[i] != '\0') {
    char c = arg[i];
    switch (c) {
    case 'c':
      conf.color = !conf.color;
      break;
    case 'n':
      conf.lines = !conf.lines;
      break;
    case 'h':
      help();
      exit(EXIT_SUCCESS);
      break;
    case 'V':
      version();
      exit(EXIT_SUCCESS);
      break;
    default: {
      char *str = malloc(2);
      str[0] = c;
      str[1] = '\0';
      argerr("unrecognized flag", str);
      free(str);
      break;
    }
    }
    i++;
  }
}

int parseargs(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) { // offset for argv[0]
    char *arg = argv[i];

    if (arg[0] == '-' && arg[1] == '\0') {
      return i;
    }

    if (arg[0] == '-') {
      if (arg[1] == '-') {
        parselongarg(arg);
        continue;
      }

      parseshortarg(arg);
    } else {
      return i;
    }
  }

  return i;
}
