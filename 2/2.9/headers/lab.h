#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
int my_len_double(double number);
void to_common_fraction(double number, int* numerator, int* denumerator, int base);
status_code fill_by_primes(int** prime_nums, int* size, int number);
status_code is_finite_representation(int base, int denumerator, bool* result);
status_code check_finite(double** res, int base, int* size, int count,...);
void print_arr(double arr[], int size);
#endif