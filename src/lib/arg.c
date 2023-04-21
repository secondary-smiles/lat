#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
      "\t-c\t toggle color\n"
      "\t-l\t toggle line numbers\n"
      "\t-t\t toggle file info headers\n"
      "\t-b\t toggle binary mode, -b forces binary and -bb forces NOT binary\n"
      "\t-r\t print everything to stdout (or equivalent)\n"
      "\t-p\t print file with the pager (uses less)\n"
      "\t-n\t set the name of the file in the title\n"
      "\t-V\t show program version\n"
      "\t-h\t display this help text\n\n");
  printf("environment:\n"
         "\tNO_COLOR, see https://no-color.org/\n");
}

void examples(void) {
  printf(
      "examples:\n"
      "\tlat file1\n\t\t print the contents of file1 with the default "
      "formatting\n"
      "\tlat - file1\n\t\t read from stdin (the '-' flag reads from "
      "stdin) "
      "and then print the contents of stdin and file1\n"
      "\tlat -nc file1 file2\n\t\t print the contents of file1 and "
      "file2 "
      "without printing line numbers or colors\n"
      "\tlat --binary file.txt\n\t\t force file.txt to be treated as a binary "
      "file\n"
      "\tlat -bb --pager file.txt\n\t\t force file.txt to NOT be treated "
      "as a binary file and print it in the pager\n"
      "\tcurl example.com | lat\n\t\t pipe the results of 'curl example.com' "
      "into lat\n"
      "\tfzf --preview 'lat -l {}'\n\t\t use lat as the file viewer in fzf\n");
}

void version(void) {
  printf("lat - v%s built %s at %s\n", LAT_VERSION, __DATE__, __TIME__);
}

void argerr(void) {
  printf("\n%s\n", LAT_USAGE);
  printf("run '--help' for more information\n");
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
