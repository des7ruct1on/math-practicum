#include "../headers/lab.h"

bool compare_double_less(double a, double b, double epsilon) {
    return (a - b) < epsilon;
}

bool compare_double_bigger(double a, double b, double epsilon) {
    return (a - b) > epsilon;
}

bool compare_double_equal(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

status_code factorial(int num, double* res, double epsilon) {
    if (num < 0) {
        res = NULL; 
        return code_invalid_parameter;
    }
    if (num == 0 || num == 1) {
        *res = 1;
        return code_succes;
    }
    long double prev_res = 1;
    *res = 1;
    for (int i = 2; i <= num; i++) {
        *res *= i; 
        if (compare_double_less(*res, prev_res, epsilon)) {
            return code_overflow;
        }
        prev_res = *res;
    }
    return code_succes;
}

status_code double_factorial(int num, double* res, double epsilon) {
    if (num < 0) {
        res = NULL; 
        return code_invalid_parameter;
    }
    if (num == 0 || num == 1) {
        *res = 1;
        return code_succes;
    }
    long prev_res = 1;
    *res = 1;
    if (num % 2 == 0) {
        for (int i = 2; i <= num; i += 2) {
            *res *= i; 
            if (compare_double_less(*res, prev_res, epsilon)) {
                return code_overflow;
            }
            prev_res = *res;
        }
    } else {
        for (int i = 3; i <= num; i += 2) {
            *res *= i; 
            if (*res < prev_res) {
                return code_overflow;
            }
            prev_res = *res;
        } 
    }
    return code_succes;
}


status_code binary_power(double x, int n, double* res) {
    double result = 1.0;
    while (n > 0) {
        *res = result;
        if (*res > result) {
            return code_overflow;
        }
        if (n % 2 == 1) {
            (result) *= x;
        }
        x *= x;
        n /= 2;
    }
    *res = result;
    return code_succes;
}


status_code summary_a(double epsilon, double x, double* result) {
    double sum = 0;
    int n = 0;
    double fact_res;
    double pow_res;
    int counter = 0;
    while (true) {
        if (counter > 150) {
            return code_diverge;
        }
        switch (factorial(n, &fact_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(x, n, &pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        if (n == 0) {
            *result =  pow_res / fact_res;
        } else {
            sum = *result;
            *result +=  pow_res / fact_res;
            if (compare_double_equal(sum, *result, epsilon)) {
                break;
            }
        }
        n++;
        counter++;
    }
    return code_succes;

}

status_code summary_b(double epsilon, double x, double* result) {
    double sum = 0;
    int n = 0;
    double fact_res;
    double first_pow_res;
    double second_pow_res;
    int counter = 0;
    while (true) {
        if (counter > 150) {
            return code_diverge;
        }
        switch (factorial(2 * n, &fact_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(-1, n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(x, 2 * n, &second_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        if (n == 0) {
            *result =  (first_pow_res * second_pow_res) / fact_res;
        } else {
            sum = *result;
            *result +=  (first_pow_res * second_pow_res) / fact_res;
            if (compare_double_equal(sum, *result, epsilon)) {
                break;
            }
        }
        n++;
        counter++;
    }
    return code_succes;
}

status_code summary_c(double epsilon, double x, double* result) {
    double sum = 0;
    int n = 0;
    double fact_res;
    double first_pow_res;
    double second_pow_res;
    double third_pow_res;
    int counter = 0;
    while (true) {
        if (counter > 150) {
            return code_diverge;
        }
        switch (factorial(3 * n, &fact_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (factorial(n, &second_pow_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(3, 3 * n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(second_pow_res, 3, &second_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(x, 2 * n, &third_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        if (n == 0) {
            *result =  (first_pow_res * second_pow_res * third_pow_res) / fact_res;
        } else {
            sum = *result;
            *result +=  (first_pow_res * second_pow_res * third_pow_res) / fact_res;
            if (compare_double_equal(sum, *result, epsilon)) {
                break;
            }
        }
        n++;
        counter++;
    }
    return code_succes;
}

status_code summary_d(double epsilon, double x, double* result) {
    double sum = 0.0;
    int n = 1;
    double fact_res;
    double first_pow_res;
    double second_pow_res;
    double third_pow_res;
    int counter = 0;
    while (true) {
        if (counter > 150) {
            return code_diverge;
        }
        switch (double_factorial(2 * n, &fact_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (double_factorial(2 * n - 1, &second_pow_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(-1, n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        switch (binary_power(x, 2 * n, &third_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            default:
                break;
        }
        if (n == 1) {
            *result =  (first_pow_res * second_pow_res * third_pow_res) / fact_res;
        } else {
            sum = *result;
            *result +=  (first_pow_res * second_pow_res * third_pow_res) / fact_res;
            if (compare_double_equal(sum, *result, epsilon)) {
                break;
            }
        }
        n++;
        counter++;
    }
    return code_succes;
}