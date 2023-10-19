#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#define EPS 1e-7
typedef enum status_code {
    code_success,
    code_overflow,
    code_invalid_parameter,
    code_nan
} status_code;

status_code evaluate_polynomial(double x, double* res, int count,...);
status_code binary_pow(double* res, double value, int degree);

#endif