#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"
#include "types.h"
#include "util.h"

struct line runextension(struct line *data, char *filename, size_t n) {
  struct line line;

  int in[2];
  int out[2];
  if (pipe(out) == -1 || pipe(in) == -1)
    die("pipe");

  pid_t pid = fork();
  if (pid == -1)
    die("fork");

  if (pid == 0) {
    // child
    dup2(out[1], STDOUT_FILENO);
    close(out[0]);
    close(out[1]);

    dup2(in[1], STDIN_FILENO);
    close(in[0]);
    close(in[1]);

    char *eargv[4];
    eargv[0] = conf.extension;
    eargv[1] = filename;
    eargv[2] = malloc((intlen(n) + 1) * sizeof(char));
    if (eargv[2] == NULL)
      die("malloc");
    sprintf(eargv[2], "%zu", n);
    eargv[3] = NULL;

    if (execvp(conf.extension, eargv) == -1)
      die("exec");
  } else {
    // parent
    close(out[1]);
    close(in[1]);

    write(in[0], "test\0", 4);

    char *buf = malloc(256);
    memset(buf, '\0', 256);

    size_t len = read(out[0], buf, 256);

    line.buf = buf;
    line.len = len;

    int status;
    waitpid(pid, &status, 0);

    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
      die("exec fail");
    }
  }

  close(out[0]);
  close(out[1]);
  close(in[0]);
  close(in[1]);

  return line;
}
