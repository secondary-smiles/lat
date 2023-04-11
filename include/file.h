#include <stdio.h>

#ifndef FILE_H
#define FILE_H
struct filedata {
  int lc;
  unsigned len;
  char *buf;
};

struct filedata readfile(FILE *fp);
#endif
