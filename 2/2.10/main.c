#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
//#include "headers/lab.h"

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_alloc_error
} status_code;

void print_coefs(double* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("g(%d) = %f\n", i, arr[i]);
    }
}

void coeffs_derivative(int degree, double coeffs[]) {
    for (int i = 0; i < degree; i++) {
        coeffs[i] = coeffs[i + 1] * (i + 1);
    }
}

status_code solve_polynomial(double* res, double point, int degree, double coefs[]) {
    *res = 0;
    int cur_degree = 1;
    for (int i = 0; i < degree + 1; i++) {
        *res += coefs[i] * cur_degree;
        cur_degree *= point;
        //printf("%f--\n\n", *res);
    }
    return code_success;

}

status_code solve_coefs(double a, double** coefs_g, int* size_coefs, int degree,...) {
    if (degree < 0) {
        return code_invalid_parameter;
    }
    va_list ptr;
    va_start(ptr, degree);
    (*coefs_g) = (double*)malloc(sizeof(double) * (degree + 1));
    if (*coefs_g == NULL) {
        return code_alloc_error;
    }
    double coefs[degree + 1];
    for (int i = 0; i < degree + 1; i++) {
        coefs[i] = va_arg(ptr, double);
    }
    va_end(ptr);
    double factorial_tmp = 1;
    for (int i = 0; i < degree + 1; i++) {
        double coefficent_g;
        status_code st_polynomial = solve_polynomial(&coefficent_g, a, degree - i, coefs);
        (*coefs_g)[i] = coefficent_g;
        if (i > 1) {
            factorial_tmp *= i;
            (*coefs_g)[i] /= factorial_tmp;
        }
        coeffs_derivative(degree - i, coefs);
    }
    return code_success;
}
int main(int argc, char* argv[]) {
    double* coefs_g = NULL;
    int size_coefs;
    int degree = 5;
    switch (solve_coefs(1, &coefs_g, &size_coefs, degree, -112.0, 7.0, -2.0, 0.0, 3.0, 1.0)) {
        case code_success:
            print_coefs(coefs_g, degree + 1);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(1);
        case code_alloc_error:
            printf("Error malloc detected!!!\n");
            exit(2);
    }
    degree = 3;
    printf("\n");
    switch (solve_coefs(2, &coefs_g, &size_coefs, degree, 2.0, 0.0, 3.0, 4.0)) {
        case code_success:
            print_coefs(coefs_g, degree + 1);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(1);
        case code_alloc_error:
            printf("Error malloc detected!!!\n");
            exit(2);
    }
    degree = 0;
    printf("\n");
    switch (solve_coefs(2, &coefs_g, &size_coefs, degree, -4.1)) {
        case code_success:
            print_coefs(coefs_g, degree + 1);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(1);
        case code_alloc_error:
            printf("Error malloc detected!!!\n");
            exit(2);
    }
    return 0;
}