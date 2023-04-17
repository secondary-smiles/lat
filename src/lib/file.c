#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"

struct filedata readfile(FILE *fp) {
  struct filedata f;

  f.lc = 0;
  f.buflen = 0;
  f.binary = 0;

  f.buf = NULL;
  f.lines = NULL;

  // expects to be at  beginning of file
  fseek(fp, 0, SEEK_END);
  f.buflen = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  f.buf = malloc(f.buflen);
  if (f.buf == NULL)
    die("malloc");

  if (fread(f.buf, f.buflen, 1, fp) < 0) {
    die("fread");
  }

  f.lc = 10000;

  // guess if printable
  // from https://github.com/sharkdp/content_inspector/blob/master/src/lib.rs
  int testlen = f.buflen >= 64 ? 64 : f.buflen;
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
