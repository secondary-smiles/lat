#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>

#include "arg.h"
#include "util.h"

#define LAT_SHORT_ARGS "cltbrpn:e:Vh"
#define LAT_USAGE "usage: lat [-cltbrpneVh] [file...]"

struct config conf;

void help(void) {
  printf("lat | lazy cat - a cat clone with some quality-of-life "
         "embellishments\n\n");

  printf("%s\n\n", LAT_USAGE);
  printf("options:\n"
         "\t-c           toggle color\n"
         "\t-l           toggle line numbers\n"
         "\t-t           toggle file info headers\n"
         "\t-b           set binary mode, -b forces binary and -bb forces NOT "
         "binary\n"
         "\t-r           print everything (headers, line numbers, etc.) to "
         "stdout (or equivalent)\n"
         "\t-p           print file with the pager (uses less)\n"
         "\t-n <name>    manually set the name of the file shown in the title\n"
         "\t-e <program> link extension to lat\n"
         "\t-V           show program version\n"
         "\t-h           display this help text\n\n");
  printf("environment:\n"
         "\tNO_COLOR, see https://no-color.org/\n");
}

void version(void) {
  printf("lat - v%s built %s at %s\n", LAT_VERSION, __DATE__, __TIME__);
}

noreturn void argerr(void) {
  printf("\n%s\n", LAT_USAGE);
  printf("run '-h' for more information\n");
  exit(EXIT_FAILURE);
}

int parseargs(int argc, char *argv[]) {
  char opt;
  while ((opt = getopt(argc, argv, LAT_SHORT_ARGS)) != -1) {
    switch (opt) {
    case 'c':
      conf.color = !conf.color;
      break;
    case 'l':
      conf.lines = !conf.lines;
      break;
    case 't':
      conf.headers = !conf.headers;
      break;
    case 'b':
      if (conf.force_binary < 0)
        conf.force_binary = 1;
      else
        conf.force_binary = !conf.force_binary;
      break;
    case 'r':
      conf.literal = !conf.literal;
      break;
    case 'p':
      conf.pager = !conf.pager;
      break;
    case 'n':
      conf.name = optarg;
      break;
    case 'e':
      conf.extension = optarg;
      break;
    case 'V':
      version();
      exit(EXIT_SUCCESS);
      break;
    case 'h':
      help();
      exit(EXIT_SUCCESS);
      break;
    default:
      argerr();
      break;
    }
  }
  return optind;
}
