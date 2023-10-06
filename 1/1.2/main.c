#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Your programm must start with: %s <ACCURACY_PARAM>", argv[0]);
        exit(1);
    }
    int l = atof(argv[1]);
    double epsilon = pow(10, -l);
    double result_gamma_equation;
    printf("e-const\n");
    printf("\tlimit: %.*Lf\n", l, limit(function_exp_limit, epsilon));
    printf("\trow: %.*Lf\n", l, summ_row(function_exp_row, epsilon, 0));
    printf("\tequation: %.*f\n", l, equation_dichotomy(function_exp_equation, 2, 3, epsilon));
    printf("pi-const\n");
    printf("\tlimit: %.*Lf\n", l, function_pi_limit(epsilon));
    printf("\trow: %.*Lf\n", l, summ_row(function_pi_row, epsilon, 1));
    printf("\tequation: %.*f\n", l, equation_dichotomy(function_pi_equation, 2, 3.5, epsilon));
    printf("sqrt\n");
    printf("\tlimit: %.*Lf\n", l, function_sqrt2_lim(epsilon));
    printf("\trow: %.*Lf\n", l, prod(function_sqrt2_row, epsilon, 2));
    printf("\tequation: %.*f\n", l, equation_dichotomy(function_sqrt_equation, 1, 2, epsilon));
    printf("gamma\n");
    printf("\tlimit: %.*Lf\n", l, function_gamma_limit(epsilon));
    printf("\trow: %.*f\n", l, function_gamma_row());
    switch(function_gamma_equation(epsilon, &result_gamma_equation)) {
        case code_success:
            printf("\tequation: %.*f\n", l, result_gamma_equation);
            break;
        case code_invalid_param:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_error_malloc:
            printf("Error mallocation detected!!!\n");
            break;
    }

    return 0;
}