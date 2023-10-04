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

void generate_lexems(FILE* input_file, FILE* output_file);
void rewrite_by_two_files(FILE* input_file1, FILE* input_file2, FILE* output_file);


#endif