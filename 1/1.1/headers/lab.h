#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>

// constants
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

// functions 
status_code factorial(int num, long* res);
status_code print_natural_number(int num);
void partition_number(const char* num);
status_code is_prime_num(int num, bool* result);
status_code print_pow(int num_pow, int num);
status_code ariphm_progression(int num, int* result);
#endif