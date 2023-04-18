#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "util.h"

#define LAT_USAGE "usage: lat [-cntbVh] [file...]"

void help(void) {
  printf("lat | lazy cat - a cat clone with some quality-of-life "
         "embelishments\n\n");

  printf("%s\n\n", LAT_USAGE);
  printf("options:\n"
         "\t-c, --color\t toggle whether to print color or not\n"
         "\t-n, --lines\t toggle whether to print line numbers or not\n"
         "\t-t, --headers\t toggle whether to print file headers or not\n"
         "\t-b, --binary\t toggle whether to force the data to be treated as "
         "binary or not\n"
         "\t-V, --version\t show program version\n"
         "\t-h, --help\t display this help text\n\n");
  printf("environment:\n"
         "\tNO_COLOR, see https://no-color.org/\n\n");
  printf("examples:\n"
         "\tlat file1\n\t\t print the content of file1 witht default formatting\n"
         "\tlat - file1\n\t\t read from stdin (the '-' character reads from stdin) "
         "and then print the contents of stdin and file1\n"
         "\tlat -nc file1 file2\n\t\t print the contents of file1 and file2 "
         "without printing line numbers or colors\n"
         "\tcurl example.com | lat\n\t\t pipe the results of 'curl example.com' "
         "into lat\n");
}

void version(void) {
  printf("lat - v%s built %s at %s\n", LAT_VERSION, __DATE__, __TIME__);
}

struct config conf;
void argerr(char *r, char *arg) {
  printf("lat: %s '%s'\n\n", r, arg);

  printf("%s\n", LAT_USAGE);
  printf("run '--help' for more information\n");
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

  if (strcmp(arg, "--headers") == 0) {
    conf.headers = !conf.headers;
    return;
  }

  if (strcmp(arg, "--binary") == 0) {
    conf.force_binary = !conf.force_binary;
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
    case 't':
      conf.headers = !conf.headers;
      break;
    case 'b':
      conf.force_binary = !conf.force_binary;
      break;
    case 'V':
      version();
      exit(EXIT_SUCCESS);
      break;
    case 'h':
      help();
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
