#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void die(const char *message) {
  perror(message);
  exit(EXIT_FAILURE);
}

char *formatbytes(size_t bytes, float *rounded) {
  char *SIZES[] = {"bytes", "kB", "MB", "GB"};

  size_t size = bytes;
  size_t div = 0;
  size_t rem = 0;

  while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
    rem = (size % 1024);
    div++;
    size /= 1024;
  }

  *rounded = (float)size + (float)rem / 1024.0;

  return SIZES[div];
}

int intlen(size_t i) {
  int l = 1;

  while (i > 9) {
    l++;
    i /= 10;
  }

  return l;
}
