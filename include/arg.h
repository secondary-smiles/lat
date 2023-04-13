#ifndef ARG_H
#define ARG_H
#include <stdbool.h>

struct config {
  bool color;
  bool lines;
};

extern struct config conf;

int parseargs(int argc, char *argv[]);

#endif
