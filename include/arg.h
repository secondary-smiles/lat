#ifndef ARG_H
#define ARG_H
#include <stdbool.h>

#define LAT_VERSION "0.9.2"

struct config {
  bool stdin;
  bool process;
  bool color;
  bool lines;
  bool headers;
  bool force_binary;
  bool has_read_stdin;
};

extern struct config conf;

int parseargs(int argc, char *argv[]);

#endif
