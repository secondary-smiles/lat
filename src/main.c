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

FILE *st;
FILE *err;

void run(FILE *fp, char *filename, bool tty) {
  const char *invert_t = conf.color ? INVERT_T : "";
  const char *grey = conf.color ? GREY : "";
  const char *reset = conf.color ? RESET : "";

  struct filedata f;
  f = readfile(fp, conf.stdin);

  if (conf.pager) {
    st = popen("less", "w");
    err = st;

    if (st == NULL)
      die("popen 'less'");
  }

  if (conf.literal) {
    err = st;
  }

  if (conf.force_binary > 0) {
    f.binary = true;
  } else if (conf.force_binary == 0) {
    f.binary = false;
  }

  if (conf.headers) {
    char *addon = f.binary ? "<binary>" : "";
    if (conf.stdin && !conf.pager)
      fprintf(err, "\x1b[2K\r%s%s%s%s\n", invert_t, basename(filename), addon,
              reset);
    else
      fprintf(err, "%s%s%s%s\n", invert_t, basename(filename), addon, reset);
  }

  conf.process = (tty && !f.binary);
  if (conf.process) { // file display processing
    loadlines(&f);
  }

  if (conf.process) {
    int linecount = 1;
    for (int i = 0; i < f.lc; i++) {
      if (conf.lines) {
        char *padding = linepad(linecount, f.lc);
        fprintf(st, "%s%s%d│%s ", grey, padding, i + 1, reset);
        fwrite(f.lines[i].buf, 1, f.lines[i].len, st);
        fprintf(st, "\n");
        free(padding);
        linecount++;
      } else {
        fprintf(st, "%s\n", f.lines[i].buf);
      }
      free(f.lines[i].buf);
    }
  } else {
    fwrite(f.buf, 1, f.buflen, st);
    fflush(st);
    fwrite("\n", 1, 1, err);
  }
  free(f.buf);

  fflush(st); // prevent timing inconsistencies between st and err
  fflush(err);

  if (conf.headers) {
    float rounded;
    char *format = formatbytes(f.buflen, &rounded);

    fprintf(err, "%s%.2f %s%s\n", invert_t, rounded, format, reset);
  }

  if (conf.pager) {
    pclose(st); // err is already the same as st
  }
}

void initconf(void) {
  conf.force_binary = -1;
  conf.stdin = false;
  conf.has_read_stdin = false;
  conf.pager = false;
  conf.literal = false;
  conf.process = true;
  conf.headers = true;
  conf.color = true;
  conf.lines = true;
}

void clearstdin(void) {
  // from
  // https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
  fseek(stdin, 0, SEEK_END);
}

int main(int argc, char *argv[]) {
  initconf();

  st = stdout;
  err = stderr;

  // init no_color first so that args take priority
  char *no_color = getenv("NO_COLOR");

  if (no_color != NULL && no_color[0] != '\0') {
    conf.color = false;
  }

  bool tty = isatty(STDOUT_FILENO);

  if (argc > 1) {
    int offset = parseargs(argc, argv);
    tty = tty || conf.literal;
    conf.headers = conf.headers && tty; // tty still overrides user
    conf.pager = conf.pager && tty;

    for (int i = offset; i < argc; i++) {
      if (*argv[i] == '-') {
        if (conf.has_read_stdin)
          clearstdin();
        conf.has_read_stdin = true;
        conf.stdin = true;
        run(stdin, "stdin", tty);
        continue;
      }

      conf.stdin = false;
      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL)
        die(argv[i]);
      run(fp, argv[i], tty);
      fclose(fp);
      if (tty && (i + 1 != argc)) {
        printf("offset: %d argc: %d\n", i, argc);
        fprintf(err, "\n"); // separate concurrent files in tty
      }
    }

    if (offset == argc) {
      conf.stdin = true;
      run(stdin, "stdin", tty);
    }
  } else {
    conf.stdin = true;
    run(stdin, "stdin", tty); // for piped-input or repl-like behavior
  }

  return EXIT_SUCCESS;
}
