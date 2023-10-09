#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    double res_geom;
    double res_pow;
    switch(geometric_mean(&res_geom, 4, 2.0, 3.0, 4.0, 5.0)) {
        case code_success:
            printf("Result = %f\n", res_geom);
            break;
        case code_overflow:
            printf("Overflow detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_nan:
            printf("NaN caught!!!\n");
            break;
    }
    printf("\n\n");
    double num_to_pow;
    int degree;
    printf("Enter number to pow: ");
    scanf("%lf", &num_to_pow);
    printf("Enter a power: ");
    scanf("%d", &degree);
    switch(binary_pow(&res_pow, num_to_pow, degree)) {
        case code_success:
            printf("Result = %f\n", res_pow);
            break;
        case code_overflow:
            printf("Overflow detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_nan:
            printf("NaN caught!!!\n");
            break;
    }
    return 0;
}