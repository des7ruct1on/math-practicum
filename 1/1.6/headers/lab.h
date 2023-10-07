#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define EPS 10e-4

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_overflow
} status_code;

status_code function_a(double x, long double* result);
status_code function_b(double x, long double* result);
status_code function_c(double x, long double* result);
status_code function_d(double x, long double* result);
status_code midpoint_rule_with_precision(status_code function(double x, long double* res), long double* res, double a, double b, double epsilon);


#endif