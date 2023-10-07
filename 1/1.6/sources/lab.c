#include "../headers/lab.h"

status_code function_a(double x, long double* result) {
    if (x == 0 || x <= -1) {
        result = NULL;
        return code_invalid_parameter;
    }
    *result = log(1 + x) / x;
    return code_success;
}

status_code function_b(double x, long double* result) {
    *result = pow(exp(1), -x * x / 2);
    return code_success;
}

status_code function_c(double x, long double* result) {
    if (x >= 1) {
        result = NULL;
        return code_invalid_parameter;
    }
    *result = log(1 / (1 - x));
    return code_success;
}

status_code function_d(double x, long double* result) {
    *result = pow(x, x);
    return code_success;
}

status_code midpoint_rule_with_precision(status_code function(double x, long double* res), long double* res, double a, double b, double epsilon) {
    int num_intervals = 1; 
    long double integral_prev = 0.0;
    long double integral = epsilon + 1.0; 

    while (fabs(integral - integral_prev) > epsilon) {
        integral_prev = integral;
        double length_interval = (b - a) / num_intervals;
        integral = 0.0;

        for (int i = 0; i < num_intervals; ++i) {
            double x_midpoint = a + (i + 0.5) * length_interval;
            long double tmp_res;
            switch (function(x_midpoint, &tmp_res)) {
                case code_invalid_parameter:
                    res = NULL;
                    return code_invalid_parameter;
                case code_success:
                    break;
                default:
                    break;
            }
            integral += tmp_res;
            tmp_res = 0;
        }
        if (num_intervals > INT_MAX / 2) {
            return code_overflow;
        }
        integral *= length_interval;
        num_intervals *= 2; 
    }
    *res = integral;
    return code_success;
}