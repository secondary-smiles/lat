#ifndef ARG_H
#define ARG_H
#include <stdbool.h>

#define LAT_VERSION "0.12.1"

struct config {
  bool stdin;
  bool process;
  bool color;
  bool lines;
  bool headers;
  int force_binary;
  bool literal;
  bool pager;
  char *name;
  bool has_read_stdin;
};

extern struct config conf;

int parseargs(int argc, char *argv[]);

#endif
