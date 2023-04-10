#include <stdio.h>
#include <stdlib.h>

#include "lib.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage: catclone <FILE>\n");
    die("args");
  }

  FILE *fp = fopen(argv[1], "r");

  if (fp == NULL) {
    die("fopen");
  }

  fclose(fp);

  return 0;
}
