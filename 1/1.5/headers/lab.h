#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef enum status_code {
    code_succes,
    code_overflow,
    code_invalid_parameter,
    code_diverge
} status_code;

bool compare_double_less(double a, double b, double epsilon)
bool compare_double_bigger(double a, double b, double epsilon);
bool compare_double_equal(double a, double b, double epsilon);
status_code factorial(int num, double* res, double epsilon);
status_code double_factorial(int num, double* res, double epsilon);
status_code binary_power(double x, int n, double* res);
status_code summary_a(double epsilon, double x, double* result);
status_code summary_b(double epsilon, double x, double* result);
status_code summary_c(double epsilon, double x, double* result);
status_code summary_d(double epsilon, double x, double* result);

#endif