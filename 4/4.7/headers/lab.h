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
#include <time.h>
#define MIN_SIZE 64

typedef struct MemoryCell {
    char* var;
    int value;
} MemoryCell;

typedef struct Storage {
    MemoryCell* data;
    int size;
    int capacity;
} Storage;

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum Operator {
    PLUS,
    MINUS,
    MULT,
    DIV,
    MOD,
    POW,
    INVALID
} Operator;

status_code create_storage(Storage** st);
status_code resize_storage(Storage** st);
void free_cell(MemoryCell* cell);
void free_storage(Storage* st);
status_code create_cell(MemoryCell** cell, char* name, int val);
int compare_cells(const void* a, const void* b);
int get_value_index(Storage* st, int index);
int binary_search(Storage* st, char* key);
status_code add_cell(Storage** st, char* name, int value);
status_code edit_var(Storage* st, char* name, int new_val);
void print_storage(Storage* st);
void print(Storage* st, char* name);
Operator get_op(char symb);
bool is_number_str(char* str);
status_code process_line(char* line, Storage* st);
status_code solve_equation(Operator op, int a, int b, int* result);
void print_error(status_code st);
#endif