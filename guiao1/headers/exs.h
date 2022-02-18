#ifndef EXS_H
#define EXS_H

#include "consts.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int cat();
int cp(const char* src, const char* dst, size_t buffer_size);
int println(const char* file);
int nl();

#endif