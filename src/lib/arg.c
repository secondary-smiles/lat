#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdnoreturn.h>

#include "arg.h"
#include "util.h"

#define LAT_SHORT_ARGS "cltbrpn:Vh"
#define LAT_USAGE "usage: lat [-cltbrpnVh] [file...]"

struct config conf;

void help(void) {
  printf("lat | lazy cat - a cat clone with some quality-of-life "
         "embellishments\n\n");

  printf("%s\n\n", LAT_USAGE);
  printf(
      "options:\n"
      "\t-c\t\t toggle color\n"
      "\t-l\t\t toggle line numbers\n"
      "\t-t\t\t toggle file info headers\n"
      "\t-b\t\t set binary mode, -b forces binary and -bb forces NOT binary\n"
      "\t-r\t\t print everything (headers, line numbers, etc.) to stdout (or equivalent)\n"
      "\t-p\t\t print file with the pager (uses less)\n"
      "\t-n <name>\t\t manually set the name of the file in the title\n"
      "\t-V\t\t show program version\n"
      "\t-h\t\t display this help text\n\n");
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
