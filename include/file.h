#ifndef FILE_H
#define FILE_H

#include <stdbool.h>
#include <stdio.h>

struct filedata readfile(FILE *fp, bool isstdin);
#endif
