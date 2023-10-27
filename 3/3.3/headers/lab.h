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

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

typedef struct Employee {
    unsigned int id;
    char name[256];
    char surname[256];
    double salary;
} Employee;

typedef enum Type_sort {
    ASCENDING,
    DESCENDING
} Type_sort;

status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
status_code read_from_file(FILE* in, Employee** storage, int* capacity);
int compare_employees(const void* a, const void* b);
void write_file(FILE* out, Employee* storage, int size, Type_sort order_status);
#endif