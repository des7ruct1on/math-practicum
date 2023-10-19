#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

typedef enum status_func {
    func_success,
    func_error
} status_func;

status_func equation_dichotomy(double function(double x), double a, double b, double eps, double* result) {
    if (a > b) {
        return func_error;
    }
    double mid_res;
    if (function(a) * function(b) < 0) {
        while (fabs(a - b) > eps) {
            mid_res = (a + b) / 2;
            if (function(a) * function(mid_res) > 0) {
                a = mid_res;
            } else if (function(b) * function(mid_res) > 0) {
                b = mid_res;
            } else {
                *result = mid_res;
                return func_success;
            }
        }
        *result = (a + b) / 2;
        return func_success;
    } else {
        return func_error;
    }
}

double func1 (double x) {
    return x - 1 / (3 + sin(3.6*x)) ;
}
double func2 (double x) {
    return ((pow(x, 2) * 0.1) - (x * log(x)));
}

double func3 (double x) {
    return cos(x);
}

double func4(double x) {
    return pow(x, 2) - 4;
}

double func5(double x) {
    return log2(x) - 4;
}

int main(int argc, char* argv[]) { 
    double epsilon;
    printf("Enter epsilon value: ");
    scanf("%lf", &epsilon);
    double res;
    status_func dich_status;
    dich_status = equation_dichotomy(func1, 0, 0.85, epsilon, &res);
    if (dich_status == func_error) {
        printf("Can`t solve this equation\n");
    }
    printf("function_1 res = %f\n", res);
    dich_status = equation_dichotomy(func2, 1, 2, epsilon, &res);
    if (dich_status == func_error) {
        printf("Can`t solve this equation\n");
    }
    printf("function_2 res = %f\n", res);
    dich_status = equation_dichotomy(func3, 0.5, 4, epsilon, &res);
    if (dich_status == func_error) {
        printf("Can`t solve this equation\n");
    }
    printf("function_3 res = %f\n", res);
    dich_status = equation_dichotomy(func4, 1, 3, epsilon, &res);
    if (dich_status == func_error) {
        printf("Can`t solve this equation\n");
    }
    printf("function_4 res = %f\n", res);
    dich_status = equation_dichotomy(func5, 15, 20, epsilon, &res);
    if (dich_status == func_error) {
        printf("Can`t solve this equation\n");
    }
    printf("function_5 res = %f\n", res);
    return 0;
}