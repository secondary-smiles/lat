#ifndef ARG_H
#define ARG_H
#include <stdbool.h>

#define LAT_VERSION "0.13.0"

struct config {
  bool isstdin;
  bool process;
  bool color;
  bool lines;
  bool headers;
  int force_binary;
  bool literal;
  bool pager;
  char *name;
  char *extension;
  bool has_read_stdin;
};

extern struct config conf;

int parseargs(int argc, char *argv[]);

#endif
