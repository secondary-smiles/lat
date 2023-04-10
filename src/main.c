#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#define INVERT_T "\x1b[7m"
#define UINVERT_T "\x1b[27m"
#define GREY "\x1b[90m"
#define RESET "\x1b[0m"

int run(char *filename) {
  FILE *fp = fopen(filename, "r+b");

  if (fp == NULL)
    die("fopen");

  printf("%s%s%s\r\n", INVERT_T, filename, UINVERT_T);

  int bufsize = 4;
  char *buf;

  buf = malloc(bufsize);
  if (buf == NULL)
    die("malloc");

  double fsize = 0;
  unsigned offset = 0;
  unsigned lc = 0;
  char c;
  while (fread(&c, sizeof(char), 1, fp) > 0) {
    if (fsize == bufsize - 1) {
      bufsize *= 2;

      char *new_buf = realloc(buf, bufsize);
      if (buf == NULL) {
        free(buf);
        die("realloc");
      }

      buf = new_buf;
    }

    if (c == '\n')
      lc++;

    buf[offset++] = c;
    fsize++;
  }
  fclose(fp);

  int lcpad = intlen(lc);

  lc = 0;
  char pc = '\0';
  for (int i = 0; i < offset; i++) {
    c = buf[i];

    if (pc == '\n' || i == 0) {
      lc++;
      int padlen = lcpad - intlen(lc);
      char padding[padlen];
      memset(padding, '0', padlen);
      printf("%s%s%d:%s ", GREY, padding, lc, RESET);
    }

    pc = c;
    printf("%c", c);
  }

  char *format = formatBytes(&fsize);

  printf("%s%.2f %s%s\r\n", INVERT_T, fsize, format, UINVERT_T);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: catclone <FILE>\n");
    die("args");
  }

  for (int i = 1; i < argc; i++) {
    if (run(argv[i]) != 0)
      die("run");

    if (i + 1 != argc) {
      printf("\r\n");
    }
  }

  return 0;
}
