#include <stdio.h>

#ifndef FILE_H
#define FILE_H
struct filedata {
  int lc;
  size_t len;
  int binary;
  char *buf;
};

struct filedata readfile(FILE *fp);
#endif
