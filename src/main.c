#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib.h"

#define INVERT_T "\x1b[7m"
#define UINVERT_T "\x1b[27m"
#define GREY "\x1b[90m"
#define RESET "\x1b[0m"

int run(char *filename) {
  int tty = isatty(STDOUT_FILENO);

  FILE *fp = fopen(filename, "r+b");

  if (fp == NULL)
    die("fopen");

  if (tty)
    fprintf(stderr, "%s%s%s\r\n", INVERT_T, filename, UINVERT_T);

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
  for (int i = 0; (unsigned)i < offset; i++) {
    c = buf[i];

    if (pc == '\n' || i == 0) {
      lc++;
      int padlen = lcpad - intlen(lc);
      char padding[padlen];
      memset(padding, ' ', padlen);
      if (tty)
        fprintf(stderr, "%s%s%d:%s ", GREY, padding, lc, RESET);
    }

    pc = c;
    printf("%c", c);
  }

  char *format = formatBytes(&fsize);

  if (tty)
    fprintf(stderr, "%s%.2f %s%s\r\n", INVERT_T, fsize, format, UINVERT_T);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <FILE/s>\n", argv[0]);
    exit(1);
  }

  for (int i = 1; i < argc; i++) { // start at one to offset argv[0]
    if (run(argv[i]) != 0)
      die("run");

    if (i + 1 != argc) {
      fprintf(stderr, "\r\n"); // separate concurrent files
    }
  }

  return 0;
}
