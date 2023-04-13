#ifndef ARG_H
#define ARG_H
#include <stdbool.h>

#define LAT_VERSION "0.4.0"

struct config {
  bool color;
  bool lines;
  bool has_read_stdin;
};

extern struct config conf;

int parseargs(int argc, char *argv[]);

#endif
