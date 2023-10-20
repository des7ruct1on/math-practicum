#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

typedef enum status_code {
    code_success,
    code_error_open_file,
    code_invalid_parameter
} status_code;

status_code find_substr(const char* substr, FILE* file, bool* found, int* index_row, int* index_symb);
status_code find_first_in(char* substr, int count, ...);
#endif