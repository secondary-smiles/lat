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

int color = 1;

void run(FILE *fp, char *filename, int tty) {
  const char *invert_t = color ? INVERT_T : "";
  const char *uinvert_t = color ? UINVERT_T : "";
  const char *grey = color ? GREY : "";
  const char *reset = color ? RESET : "";

  struct filedata f;
  f = readfile(fp);

  if (tty) {
    char *addon = f.binary ? " <binary>" : "";
    fprintf(stderr, "\r%s%s%s%s\r\n", invert_t, basename(filename), addon,
            uinvert_t);
  }

  int lcpad = intlen(f.lc);

  f.lc = 0;
  char pc = '\0';
  char c;
  for (size_t i = 0; i < f.len; i++) {
    c = f.buf[i];

    if (tty && !f.binary && (pc == '\n' || i == 0)) {
      f.lc++;

      int padlen = lcpad - intlen(f.lc);
      char padding[padlen];

      if (padlen)
        memset(padding, ' ', padlen);

      fprintf(stderr, "\r%s%s%d:%s ", grey, padlen > 0 ? padding : "", f.lc,
              reset); // padlen  < 1 causes undefined
    }

    pc = c;
    printf("%c", c);
  }

  fflush(stdout); // prevent timing inconsistencies between stdout and stderr

  if (tty) {
    float rounded;
    char *format = formatbytes(f.len, &rounded);

    char *cnewline = c == '\n' ? "" : "\n";
    fprintf(stderr, "\r%s%s%.2f %s%s\r\n", cnewline, invert_t, rounded, format,
            uinvert_t);
  }
}

int main(int argc, char *argv[]) {
  char *no_color = getenv("NO_COLOR");

  if (no_color != NULL && no_color[0] != '\0') {
    color = 0;
  }

  if (argc > 1) {
    for (int i = 1; i < argc; i++) { // start at one to offset argv[0]
      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL)
        die(argv[i]);

      int tty = isatty(STDOUT_FILENO);
      run(fp, argv[i], tty);
      fclose(fp);

      if (i + 1 != argc) {
        fprintf(stderr, "\r\n"); // separate concurrent files
      }
    }
  } else {
    run(stdin, "stdin", 1); // for piped-input or repl-like behavior
  }

  return 0;
}
