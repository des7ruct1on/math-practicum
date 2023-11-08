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


typedef struct Vector {
    double* coordinates;
} Vector;



status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
void print_vector(Vector* arr, int size);
status_code binary_pow(double* res, double value, int degree);
status_code get_golder_norm(double* res, Vector* vect, int n, int p);
status_code get_golder_inf(double* res, Vector* vect, int n, int p);
status_code mult_matr_vect(double* a[], double* x, double** res, int size_m,  int size_v);
status_code get_matrix_norm(double* res, Vector* vect, int n, int p);
status_code get_max_len_vector(Vector** max_vectors, int* size, status_code norm(double* res, Vector* vect, int n, int p), int dimension, int p, int count,...);
#endif