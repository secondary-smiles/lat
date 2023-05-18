#include <stdio.h>
#include <stdlib.h>
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

struct colors {
  char *invert_t;
  char *grey;
  char *reset;
};

struct colors c;

void initcolor(void) {
  c.invert_t = conf.color ? INVERT_T : "";
  c.grey = conf.color ? GREY : "";
  c.reset = conf.color ? RESET : "";
}

void printheadertop(char *filename, bool binary) {
  char *name = conf.name == NULL ? filename : conf.name;
  char *addon = binary ? "<binary>" : "";
  if (conf.isstdin && !conf.pager)
    fprintf(err, "\x1b[2K\r%s%s%s%s\n", c.invert_t, name, addon, c.reset);
  else
    fprintf(err, "%s%s%s%s\n", c.invert_t, name, addon, c.reset);
}

void printheaderbottom(size_t buflen) {
  float rounded;
  char *format = formatbytes(buflen, &rounded);

  fprintf(err, "%s%.2f %s%s\n", c.invert_t, rounded, format, c.reset);
}

void run(FILE *fp, char *filename, bool tty) {
  initcolor();

  struct filedata f;
  f = readfile(fp, conf.isstdin);

  if (conf.extension != NULL) {
    // TODO
  }

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
    printheadertop(filename, f.binary);
  }

  // any/all processing to be done
  conf.process = (tty && !f.binary);
  if (conf.process) { // file display processing
    loadlines(&f);
  }

  if (conf.process) {
    int linecount = 1;
    for (int i = 0; i < f.lc; i++) {
      if (conf.lines) {
        { // line numbers
          char *padding = linepad(linecount, f.lc);
          fprintf(err, "%s%s%d│%s ", c.grey, padding, i + 1, c.reset);
          free(padding);
        }

        fwrite(f.lines[i].buf, 1, f.lines[i].len, st);
        fprintf(st, "\n");
        linecount++;
      } else {
        fprintf(st, "%s\n", f.lines[i].buf);
      }
      free(f.lines[i].buf);
    }
  } else {
    fwrite(f.buf, 1, f.buflen, st);
    fflush(st);
    if (tty)
      fwrite("\n", 1, 1, err);
  }
  free(f.buf);
  free(f.lines);

  fflush(st); // prevent timing inconsistencies between st and err
  fflush(err);

  if (conf.headers) {
    printheaderbottom(f.buflen);
  }

  if (conf.pager) {
    pclose(st); // err is already the same as st
  }
}

void initconf(void) {
  conf.force_binary = -1;
  conf.isstdin = false;
  conf.has_read_stdin = false;
  conf.pager = false;
  conf.literal = false;

  conf.process = true;
  conf.headers = true;
  conf.color = true;
  conf.lines = true;

  conf.name = NULL;
  conf.extension = NULL;
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

  int offset = parseargs(argc, argv);

  conf.headers = conf.headers && tty;

  if (argc > 1) {

    tty = tty || conf.literal;

    for (int i = offset; i < argc; i++) {
      if (*argv[i] == '-') {
        if (conf.has_read_stdin)
          clearstdin();
        conf.has_read_stdin = true;
        conf.isstdin = true;
        run(stdin, "stdin", tty);
        if (tty && (i + 1 != argc)) {
          fprintf(err, "\n"); // separate concurrent files in tty
        }

        continue;
      }

      conf.isstdin = false;
      FILE *fp = fopen(argv[i], "rb");
      if (fp == NULL)
        die(argv[i]);
      run(fp, argv[i], tty);
      fclose(fp);
      if (tty && (i + 1 != argc)) {
        fprintf(err, "\n"); // separate concurrent files in tty
      }
    }

    if (offset == argc) {
      conf.isstdin = true;
      run(stdin, "stdin", tty);
    }
  } else {
    conf.isstdin = true;
    run(stdin, "stdin", tty); // for piped-input or repl-like behavior
  }

  return EXIT_SUCCESS;
}
