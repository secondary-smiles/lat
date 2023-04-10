#include <stdio.h>
#include <stdlib.h>

void die(const char *message) {
  perror(message);
  exit(1);
}
