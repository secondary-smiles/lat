#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "util.h"

void appendline(struct filedata *f, char *data, size_t len) {
  f->lines = realloc(f->lines, sizeof(struct line) * (f->lc + 1));

  size_t loc = f->lc;

  f->lines[loc].len = len;
  f->lines[loc].buf = malloc(len + 1);
  memcpy(f->lines[loc].buf, data, len);
  f->lines[loc].buf[len] = '\0';
}

void loadlines(struct filedata *f) {
  f->lc = 0;

  size_t offset = 0;
  size_t linelen = 4096;
  char *line = malloc(linelen);
  if (line == NULL)
    die("malloc");

  for (size_t i = 0; i < f->buflen; i++) {
    char c = f->buf[i];
    if (c == '\n') {
      if (offset < linelen) {
        char *new_line = realloc(line, offset);
        if (new_line == NULL)
          die("realloc");
        line = new_line;
      }

      appendline(f, line, offset);
      f->lc++;

      free(line);
      offset = 0;
      line = malloc(linelen);
      if (line == NULL)
        die("malloc");

    } else {
      if (offset == linelen) {
        linelen *= 2;

        char *new_line = realloc(line, linelen);
        if (new_line == NULL)
          die("realloc");

        line = new_line;
      }

      line[offset++] = c;
    }
  }
}

char *linepad(int lc, int total) {
  int padlen = intlen(total) - intlen(lc);
  char *padding = malloc(padlen + 1);
  if (padding == NULL)
    die("malloc");

  if (padlen)
    memset(padding, ' ', padlen);
  padding[padlen] = '\0';

  return padding;
}
