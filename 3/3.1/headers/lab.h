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
status_code power(int base, int exponent, int* res);
int plus(int a, int b);
int minus(int a, int b);
int increment(int x);
int decrement(int x);
int abs_value(int num);
char get_char_number(int number);
int multiply(int a, int b);
status_code convert_from_decimal(long long int decimal_number, int base, char** result);
#endif