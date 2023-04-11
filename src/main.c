#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
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
    fprintf(stderr, "%s%s%s\r\n", INVERT_T, basename(filename), UINVERT_T);

  struct filedata f;
  f = readfile(fp);

  fclose(fp);

  int lcpad = intlen(f.lc);

  f.lc = 0;
  char pc = '\0';
  char c;
  for (unsigned i = 0; i < f.len; i++) {
    c = f.buf[i];

    if (pc == '\n' || i == 0) {
      f.lc++;
      int padlen = lcpad - intlen(f.lc);
      char padding[padlen];
      memset(padding, ' ', padlen);

      if (tty)
        fprintf(stderr, "%s%s%d:%s ", GREY, padding, f.lc, RESET);
    }

    pc = c;
    printf("%c", c);
  }

  float rounded;
  char *format = formatBytes(f.len, &rounded);

  if (tty)
    fprintf(stderr, "%s%.2f %s%s\r\n", INVERT_T, rounded, format, UINVERT_T);

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
