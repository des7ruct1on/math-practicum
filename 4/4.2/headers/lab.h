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

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_error_oppening,
    cmd_success
} status_cmd;

typedef struct Array {
    char name;
    int* data;
    int size;
    int capacity;
} Array;

status_code create_array(Array** ar, char c);
status_code add_to_array(Array** arr, char c, int number);
status_code remove_from_array(Array* arr, int index, int count);
status_code save_array(Array* arr, const char* filename);
void free_array(Array* arr);
status_code load_array(Array** arr, char name, const char* filename);
status_code concat_arrays(Array* a, Array* b);
void stats_array(Array* a);
Array* find_array(Array** storage, int size, char name);
status_code Free(Array* arr);
void print(Array* arr, char name, char* mode);
status_code copy_arrays(Array* a, Array** b, int start, int end);
int compare_ascending(const void* a, const void* b);
int compare_descending(const void* a, const void* b);
int compare_random(const void* a, const void* b);
int get_my_rand(int min, int max);
status_code rand_fill_array(Array** arr, char name, char* arg);
void free_storage(Array** st, int size);
void sort(Array* a, char mode);
void Shuffle(Array* a);
void add_to_storage(Array** st, Array* a, int* capacity);
status_cmd command(char* input, Array** storage, int* size_storage);
#endif