#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "headers/lab.h"

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