#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>

// constants
#define EPS 1e-7
#define HIGH_LIMIT_NATURAL_NUM 100
#define HIGH_LIMIT_PROGRESSION 65535 
/*
    (2 + (n - 1)) * (n / 2) > INT_MAX, n = 65536
*/

// enum
typedef enum status_code {
    code_succes,
    code_overflow,
    code_invalid_parameter
} status_code;

// functions (almost 1.1)
bool equal_double(double* num1, double* num2);
status_code factorial(int num, long* res);
void print_natural_number(int num);
int get_length(int num);
status_code is_prime_num(int num, bool* result);
status_code partition_number(int num, char** result, int len_number);
status_code print_pow(int num_pow, int num);
status_code ariphm_progression(int num, int* result);
#endif