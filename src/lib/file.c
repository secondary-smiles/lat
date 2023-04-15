#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "util.h"

struct filedata readfile(FILE *fp) {
  struct filedata f;

  f.lc = 0;
  f.len = 0;
  f.binary = 0;

  // expects to be at  beginning of file
  fseek(fp, 0, SEEK_END);
  f.len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  f.buf = malloc(f.len);
  if (f.buf == NULL)
    die("malloc");

  if (fread(f.buf, f.len, 1, fp) < 0) {
    die("fread");
  }

  f.lc = 10000;

  // guess if printable
  // from https://github.com/sharkdp/content_inspector/blob/master/src/lib.rs
  int testlen = f.len >= 64 ? 64 : f.len;
  char *testbuf[testlen];
  memcpy(testbuf, f.buf, testlen);

  char *result = memchr(testbuf, 0x00, testlen);

  if (result) {
    f.binary = 1;
  } else {
    f.binary = 0;
  }

  return f;
}
