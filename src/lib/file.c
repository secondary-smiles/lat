#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "util.h"

struct filedata readfile(FILE *fp) {
  struct filedata f;

  f.lc = 0;
  f.len = 0;

  size_t bufsize = 4;

  f.buf = malloc(bufsize);
  if (f.buf == NULL)
    die("malloc");

  char c;
  while (fread(&c, sizeof(char), 1, fp) > 0) {
    if (f.len == bufsize - 1) {
      bufsize *= 2;

      char *new_buf = realloc(f.buf, bufsize);
      if (f.buf == NULL) {
        free(f.buf);
        die("realloc");
      }

      f.buf = new_buf;
    }

    if (c == '\n') {
      f.lc++;
    }

    f.buf[f.len++] = c;
  }

  return f;
}
