#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"
#include "arg.h"

bool isbinary(struct filedata *f) {

  // guess if printable
  // from https://github.com/sharkdp/content_inspector/blob/master/src/lib.rs
  int testlen = f->buflen >= 64 ? 64 : f->buflen;
  char *testbuf[testlen];
  memcpy(testbuf, f->buf, testlen);

  char *result = memchr(testbuf, 0x00, testlen);

  if (result) {
    return true;
  } else {
    return false;
  }
}

struct filedata readfile(FILE *fp) {
  struct filedata f;

  f.lc = 0;
  f.buflen = 0;
  f.binary = 0;

  f.buf = NULL;
  f.lines = NULL;

  if (conf.isstdin) {
    size_t bufsize = 1024;
    f.buf = malloc(bufsize);
    if (f.buf == NULL)
      die("malloc");

    char c;
    while (fread(&c, 1, 1, fp) > 0) {
      if (f.buflen == bufsize - 1) {
        bufsize *= 2;
        char *new_buf = realloc(f.buf, bufsize);
        if (new_buf == NULL)
          die("realloc");

        f.buf = new_buf;
      }
      f.buf[f.buflen++] = c;
    }

    if (f.buflen < bufsize - 1) {
      char *new_buf = realloc(f.buf, f.buflen + 1);
      if (new_buf == NULL)
        die("realloc");

      f.buf = new_buf;
    }
    f.buf[f.buflen] = '\0';

    f.binary = isbinary(&f);

    return f;
  }

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

  f.binary = isbinary(&f);

  return f;
}
