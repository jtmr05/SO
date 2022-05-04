#ifndef TIMED_MULTIGREP_H
#define TIMED_MULTIGREP_H

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

void timed_multigrep(const char* word, const char** files, unsigned num_of_files);

#endif