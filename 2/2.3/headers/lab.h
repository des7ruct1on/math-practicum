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
    code_error_malloc,
    code_invalid_parameter
} status_code;

typedef enum status_realloc {
    status_ok,
    status_fail
} status_realloc;

typedef struct First_in {
    int index_row;
    int index_symb;
    char* filename;
} First_in;

void print_ins(First_in* ins, int size);
status_realloc my_realloc(void** var, int size);
status_code find_substr(First_in** ins, int* size, int* ind_inv, const char* filename, const char* substr, FILE* file);
status_code find_first_in(First_in** ins, int* size, char* substr, int count, ...);
#endif