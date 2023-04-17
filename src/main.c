#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "file.h"
#include "process.h"
#include "types.h"
#include "util.h"

#define INVERT_T "\x1b[7m"
#define UINVERT_T "\x1b[27m"
#define GREY "\x1b[90m"
#define RESET "\x1b[0m"

void run(FILE *fp, char *filename, bool tty) {
  const char *invert_t = conf.color ? INVERT_T : "";
  const char *grey = conf.color ? GREY : "";
  const char *reset = conf.color ? RESET : "";

  struct filedata f;
  f = readfile(fp);

  if (tty) {
    char *addon = f.binary ? "<binary>" : "";
    fprintf(stderr, "\r\x1b[2K%s%s%s%s\r\n", invert_t, basename(filename),
            addon, reset);
  }

  conf.process = (tty && !f.binary);
  if (conf.process) { // file display processing
    loadlines(&f);
  }

  if (conf.process) {
    int linecount = 0;

    for (int i = 0; i < f.lc; i++) {
      if (conf.lines) {
        char *padding = linepad(linecount, f.lc);
        printf("%s%s%d:%s %s\n", grey, padding, i, reset, f.lines[i].buf);
        free(padding);
        linecount++;
      } else {
        printf("%s\n", f.lines[i].buf);
      }

      free(f.lines[i].buf);
    }
  } else {
    printf("%s", f.buf);
  }
  free(f.buf);

  fflush(stdout); // prevent timing inconsistencies between stdout and stderr

  if (tty) {
    float rounded;
    char *format = formatbytes(f.buflen, &rounded);

    // char *cnewline = c == '\n' ? "" : "\n";
    char *cnewline = "";
    fprintf(stderr, "\r%s%s%.2f %s%s\r\n", cnewline, invert_t, rounded, format,
            reset);
  }
}

void initconf(void) {
  conf.process = true;
  conf.color = true;
  conf.lines = true;
  conf.has_read_stdin = false;
}

void clearstdin(void) {
  // from
  // https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
  fseek(stdin, 0, SEEK_END);
}

int main(int argc, char *argv[]) {
  initconf();

  // init no_color first so that args take priority
  char *no_color = getenv("NO_COLOR");

  if (no_color != NULL && no_color[0] != '\0') {
    conf.color = false;
  }

  bool tty = isatty(STDOUT_FILENO);
  if (argc > 1) {
    int offset = parseargs(argc, argv);
    for (int i = offset; i < argc; i++) {
      if (*argv[i] == '-') {
        if (conf.has_read_stdin)
          clearstdin();
        conf.has_read_stdin = true;
        run(stdin, "stdin", tty);
        continue;
      }

      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL)
        die(argv[i]);
      run(fp, argv[i], tty);
      fclose(fp);

      if (tty && (i + 1 != argc)) {
        fprintf(stderr, "\r\n"); // separate concurrent files in tty
      }
    }

    if (offset == argc) {
      run(stdin, "stdin", tty);
    }
  } else {
    run(stdin, "stdin", tty); // for piped-input or repl-like behavior
  }

  return EXIT_SUCCESS;
}
