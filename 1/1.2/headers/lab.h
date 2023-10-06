#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>

typedef enum status_code {
    code_success,
    code_error_malloc,
    code_invalid_param
} status_code;

long double limit(long double function(int x), double epsilon);
long double summ_row(long double function(int x), double epsilon, int begin);
long double prod(long double function(int x), double epsilon, int begin);
long double factorial(int n);
long double function_exp_limit(int x);
long double function_exp_row(int x);
long double function_pi_row(int x);
long double function_log_2_lim(int x);
long double function_log_2_row(int x);
long double function_sqrt2_lim(double epsilon);
double equation_dichotomy(double equation(double x), double a, double b, double eps);
long double function_sqrt2_row(int x);
double function_exp_equation(double x);
double function_pi_equation(double x);
double function_sqrt_equation(double x);
long double function_pi_limit(double eps);
long double function_gamma_limit(double epsilon);
double function_gamma_row();
status_code sieve_eratosthene(int n, bool** arr_primes);
status_code function_gamma_equation(double epsilon, double* result);
#endif