#ifndef BASH_H
#define BASH_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int mysystem(const char* command);
void bash_mkII();

#endif
