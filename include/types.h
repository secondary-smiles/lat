#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stddef.h>

struct line {
  size_t len;
  char *buf;
};

struct filedata {
  int lc;
  bool binary;

  size_t buflen;

  char *buf;
  struct line *lines;
};

#endif
