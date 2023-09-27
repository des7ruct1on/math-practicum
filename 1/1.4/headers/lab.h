#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define STR_SIZE 256

void count_latin_letters(const char* line, int* counter);
void count_non_alpha_numeric(const char* line, int* counter);
#endif