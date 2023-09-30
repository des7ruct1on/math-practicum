#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>

//enums
typedef enum sqr_equation_status_code {
    status_one_root,
    status_two_roots,
    status_no_roots,
    status_invalid_parametr
} sqr_equation_status_code;

typedef enum status_code {
    code_succes,
    code_invalid_parameter
} status_code;

//functions
bool compare_double_less(double a, double b, double epsilon);
bool compare_double_bigger(double a, double b, double epsilon);
bool compare_double_equal(double a, double b, double epsilon);
void bubble_sort_descending(double* arr, int size, double epsilon);
void swap(double *arr, int i, int j);
void reverse(double *arr, int begin, int end);
bool next_permutation(double *arr, int n);
sqr_equation_status_code solve_sqr_equation(double* coefs, double epsilon, double* root1, double* root2);
status_code is_multiply(int num1, int num2, bool* result);
status_code is_triangle(double* sides, double epsilon, bool* result);
#endif