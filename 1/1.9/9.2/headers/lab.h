#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// enum
typedef enum status_code {
    code_succes,
    code_malloc_error
} status_code;

//functions
status_code generate_array(int** arr, int size, int a, int b);
void print_array(int* arr, int size);
int find_closest_value(int target, int* arr, int size);
status_code combine_arrays(int** arr_c, int* arr_a, int* arr_b, int size_a, int size_b);
#endif