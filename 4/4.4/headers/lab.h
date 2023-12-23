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

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum condition {
    cond_value,
    cond_unknown_var,
    cond_wrong_dimensions,
    cond_error_reading,
    cond_error_writing,
    cond_met_comment,
    cond_invalid_parameter
} condition;

typedef struct Vector {
    char name;
    char* data;
    int size;
    int capacity;
    int base;
} Vector;

typedef enum status_error {
    error_missed_ending,
    error_unknown_vector,
    error_invalid_operator,
} status_error;

status_code create_vector(Vector** ar, char c);
status_code add_to_vector(Vector** arr, char c, char number);
void free_vector(Vector* arr);
Vector* find_vector(Vector** storage, int size, char name);
void free_storage(Vector** st, int size);
void add_to_storage(Vector** st, Vector* a, int* capacity);
status_code operation(Vector** a, Vector* b, char operator[]);
bool is_operator(char c);
status_code read_vector(Vector** a, int base);
status_code convert_from_decimal(int decimal_number, int base,  char** result);
status_code convert_to_decimal(char* number_str, int base, int * dec_number);
status_code print_vector(Vector* a, int base, const char* filename);
status_code process(const char* filename, Vector** storage, int* size_storage, bool is_trace, const char* output);
status_code copy_vector(Vector* a, Vector** b);
#endif