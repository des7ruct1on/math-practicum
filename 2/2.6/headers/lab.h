#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define STR_SIZE 256

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

status_realloc my_realloc(void** var, int size);
bool is_lower(char* str, int size);
bool is_upper(char* str, int size);
status_code from_roman_to_int(const char* number_str, int* num);
status_code from_zeckendorf(const char* number_z, unsigned int* number);
status_code convert_to_decimal(char* number_str, int base, int* dec_number, int flag);
int overfscanf(FILE * stream, const char * format, ...);
int oversscanf(char * buf, const char * format, ...);
#endif