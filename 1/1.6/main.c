#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <EPSILON>\n", argv[0]);
        exit(1);  
    }
    double epsilon = atof(argv[1]); // Заданная точность
    long double integral_a, integral_b, integral_c, integral_d;
    switch (midpoint_rule_with_precision(function_a, &integral_a, 0, 1, epsilon)) {
    case code_success:
        printf("Result for function_a: %Lf\n", integral_a);
        break;
    case code_invalid_parameter:
        printf("Invalid parameter detected!!!\n");
        break;
    }
    switch (midpoint_rule_with_precision(function_b, &integral_b, 0, 1, epsilon)) {
    case code_success:
        printf("Result for function_b: %Lf\n", integral_b);
        break;
    case code_invalid_parameter:
        printf("Invalid parameter detected!!!\n");
        break;
    }
    switch (midpoint_rule_with_precision(function_c, &integral_c, 0, 1, epsilon)) {
    case code_success:
        printf("Result for function_c: %Lf\n", integral_c);
        break;
    case code_invalid_parameter:
        printf("Invalid parameter detected!!!\n");
        break;
    }
    switch (midpoint_rule_with_precision(function_d, &integral_d, 0, 1, epsilon)) {
    case code_success:
        printf("Result for function_d: %Lf\n", integral_d);
        break;
    case code_invalid_parameter:
        printf("Invalid parameter detected!!!\n");
        break;
    }

    return 0;
}
