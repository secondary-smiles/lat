#include "arg.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct config conf;
void argerr(char *r, char *arg) {
  fprintf(stderr, "lat: %s '%s'\n", r, arg);
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
    default:
      argerr("unrecognized flag", &c);
      break;
    }
    i++;
  }
}

int parseargs(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; i++) { // offset for argv[0]
    char *arg = argv[i];

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
