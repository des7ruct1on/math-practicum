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
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;


typedef struct My_string {
    unsigned int size;
    char* data;
} My_string;

My_string* String(int size);
void String_clear();
status_code my_strcpy(My_string* destination, const My_string* source);
My_string* my_strcpy_new(const My_string* source);
int my_strcmp(My_string* left, My_string* right);

status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
#endif