#ifndef PERSON_H
#define PERSON_H

#include "consts.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int add_person(int age, const char* name, const char* filename);
int update_age(int age, const char* name, const char* filename);
int get_age(const char* name, const char* filename);
int update_age_index(int age, int index, const char* filename);


#endif