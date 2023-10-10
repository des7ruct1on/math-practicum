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

void count_latin_letters(FILE* input, FILE* output_file);
void count_non_alpha_numeric(FILE* input, FILE* output_file);
void remove_digits(FILE* input, FILE* output);
void remove_non_digits_by_ascii(FILE* input, FILE* output);
#endif