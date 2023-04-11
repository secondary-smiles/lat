#ifndef LIB_H
#define LIB_H
#include <stddef.h>

void die(const char *message);

char *formatbytes(size_t bytes, float *rounded);

int intlen(size_t i);
#endif
