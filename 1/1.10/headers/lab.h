#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define MAX_NUM_SIZE 32

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_malloc
} status_code;


status_code convert_to_decimal(char* number_str, int base, int* dec_number);
status_code convert_from_decimal(int decimal_number, int base,  char** result);
int remove_trailing_zeros(int number);


#endif