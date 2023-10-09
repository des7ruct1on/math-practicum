#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

typedef enum status_code {
    code_succes,
    code_malloc_error
} status_code;

int my_strlen(const char* str);
void print_s(const char* str);
int get_my_rand(int min, int max);
bool my_is_alpha(char c);
bool my_is_digit(char c);
bool my_is_upper(char c);
char my_to_upper(char c);
status_code get_reversed_string(const char* str_old, char** reversed);
status_code get_odd_to_upper_string(const char* str_old, char** upper);
status_code get_sorted_string(const char* str_old, char** sorted);
void fill_array(int arr[], int size, int a, int b, unsigned int random_seeed);
status_code get_concat_strings(const char* strings[], int count_argc, char** res_str, unsigned int random_seeed);

#endif