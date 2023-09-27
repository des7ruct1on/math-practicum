#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "headers/lab.h"

typedef enum status_code {
    code_succes,
    code_overflow,
    code_invalid_parameter,
    code_diverge
} status_code;

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
            case code_succes:
                break;
        }
        switch (binary_power(x, n, &pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
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
            case code_succes:
                break;
        }
        switch (binary_power(-1, n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(x, 2 * n, &second_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
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
            case code_succes:
                break;
        }
        switch (factorial(n, &second_pow_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(3, 3 * n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(second_pow_res, 3, &second_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(x, 2 * n, &third_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
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
            case code_succes:
                break;
        }
        switch (double_factorial(2 * n - 1, &second_pow_res, epsilon)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(-1, n, &first_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
                break;
        }
        switch (binary_power(x, 2 * n, &third_pow_res)) {
            case code_invalid_parameter:
                return code_invalid_parameter;
            case code_overflow:
                return code_overflow;
            case code_succes:
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

int main(int argc, const char *argv[]) {
    if (argc < 3) {
        printf("Your programm must start with: %s <EPSILON> <X>\n", argv[0]);
        exit(1);
    }
    double epsilon = atof(argv[1]);
    double x = atof(argv[2]);
    double res_a, res_b, res_c, res_d;
    double a;
    double_factorial(5, &a, epsilon);
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    switch (summary_a(epsilon, x, &res_a)) {
        case code_succes:
            printf("| Result of function a(x): %.10f\n", res_a);
            break;
        case code_invalid_parameter:
            printf("| Invalid parametr deteced!!!\n");
            break;
        case code_overflow:
            printf("| Result of function a(x) before overflow: %.10f\n", res_a);
            break;
        case code_diverge:
            printf("| a(x) is diverging\n");
            break;
    }
    switch (summary_b(epsilon, x, &res_b)) {
        case code_succes:
            printf("| Result of function b(x): %.10f\n", res_b);
            break;
        case code_invalid_parameter:
            printf("| Invalid parametr deteced!!!\n");
            break;
        case code_overflow:
            printf("| Result of function b(x) before overflow: %.10f\n", res_b);
            break;
        case code_diverge:
            printf("| b(x) is diverging\n");
            break;
    }
    switch (summary_c(epsilon, x, &res_c)) {
        case code_succes:
            printf("| Result of function c(x): %.10f\n", res_c);
            break;
        case code_invalid_parameter:
            printf("| Invalid parametr deteced!!!\n");
            break;
        case code_overflow:
            printf("| Result of function c(x) before overflow: %.10f\n", res_c);
            break;
        case code_diverge:
            printf("| c(x) is diverging\n");
            break;
    }
    switch (summary_d(epsilon, x, &res_d)) {
        case code_succes:
            printf("| Result of function d(x): %.10f\n", res_d);
            break;
        case code_invalid_parameter:
            printf("| Invalid parametr deteced!!!\n");
            break;
        case code_overflow:
            printf("| Result of function d(x) before overflow: %.10f\n", res_d);
            break;
        case code_diverge:
            printf("| d(x) is diverging\n");
            break;
    }
    printf("|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
    printf("| Epsilon = %f\n", epsilon);

    

    return 0;
}