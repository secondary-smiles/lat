#ifndef FILE_H
#define FILE_H

#include <stdio.h>
struct filedata {
  int lc;
  size_t len;
  int binary;
  char *buf;
  char *altbuf;
};

struct filedata readfile(FILE *fp);
#endif
