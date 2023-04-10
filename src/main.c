#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

#define INVERT_T "\x1b[7m"
#define UINVERT_T "\x1b[27m"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: catclone <FILE>\n");
    die("args");
  }

  FILE *fp = fopen(argv[1], "r+b");

  if (fp == NULL)
    die("fopen");

  printf("%s%s%s\r\n", INVERT_T, argv[1], UINVERT_T);

  int bufsize = 4;
  char *buf;

  buf = malloc(bufsize);
  if (buf == NULL)
    die("malloc");

  double fsize = 0;
  int offset = 0;
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

    buf[offset++] = c;
    fsize++;
  }

  printf("\r\n");

  char *format = formatBytes(&fsize);

  printf("%s%.2f %s%s\r\n", INVERT_T, fsize, format, UINVERT_T);

  fclose(fp);

  return 0;
}
