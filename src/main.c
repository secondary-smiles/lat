#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "util.h"

#define INVERT_T "\x1b[7m"
#define UINVERT_T "\x1b[27m"
#define GREY "\x1b[90m"
#define RESET "\x1b[0m"

void run(FILE *fp, char *filename) {
  int tty = isatty(STDOUT_FILENO);

  struct filedata f;
  f = readfile(fp);

  if (tty)
    fprintf(stderr, "\r%s%s%s\r\n", INVERT_T, basename(filename), UINVERT_T);

  int lcpad = intlen(f.lc);

  f.lc = 0;
  char pc = '\0';
  char c;
  for (size_t i = 0; i < f.len; i++) {
    c = f.buf[i];

    if (tty && (pc == '\n' || i == 0)) {
      f.lc++;

      int padlen = lcpad - intlen(f.lc);
      char padding[padlen];
      memset(padding, ' ', padlen);

      fprintf(stderr, "\r%s%s%d:%s ", GREY, padlen > 0 ? padding : "", f.lc,
              RESET); // padlen  < 1 causes undefined
    }

    pc = c;
    printf("%c", c);
  }

  if (tty) {
    float rounded;
    char *format = formatBytes(f.len, &rounded);

    fprintf(stderr, "\r%s%.2f %s%s\r\n", INVERT_T, rounded, format, UINVERT_T);
  }
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) { // start at one to offset argv[0]
      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL)
        die(argv[i]);
      run(fp, argv[i]);
      fclose(fp);

      if (i + 1 != argc) {
        fprintf(stderr, "\r\n"); // separate concurrent files
      }
    }
  } else {
    run(stdin, "stdin"); // for piped-input or repl-like behavior
  }

  return 0;
}
