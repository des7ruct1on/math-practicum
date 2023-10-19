#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define STR_SIZE 32
typedef enum status_realloc {
    status_ok,
    status_fail
} status_realloc;


typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_alloc_fail
} status_code;
status_realloc free_all(int count, ...);
status_realloc my_realloc(void** var, int size);
status_code sum_numbers_by_base(int base, char** result,int count,...);
status_code add_numbers(char** result, int base, int* size, char* number2);
status_code get_char_value(int value, int base, char* res);
void reverse_string(char* str);
status_code convert_to_decimal(char digit, int * dec_number);
bool is_valid_input(char* input_str, int base);
#endif