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
status_code to_roman(int number, char** r_value);
void fibonacci(unsigned int result[2], unsigned int number);
void zeckendorf(int* result_size, unsigned int number, unsigned int result[number][2]);
status_code convert_from_decimal(long long int decimal_number, int base, int flag,  char** result);
status_code convert_to_decimal(char* number_str, int base, long long int * dec_number, int flag);
bool is_lower(char* str, int size);
bool is_upper(char* str, int size);
int overfprintf(FILE * stream, char * format, ...);
int oversprintf(char* buf, char * format, ...);
#endif