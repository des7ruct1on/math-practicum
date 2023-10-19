#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main (int argc, const char *argv[]) {
    double res;
    switch (evaluate_polynomial(2, &res, 6, 1.0, 5.0, 6.0, 0.0, 3.0, 1.0)) {
        case code_success:
            printf("result: %f\n", res);
            break;
        default:
            printf("Invalid parameter detected!!!\n");
            break;
    }
    switch (evaluate_polynomial(2, &res, 1, 5.0)) {
        case code_success:
            printf("result: %f\n", res);
            break;
        default:
            printf("Invalid parameter detected!!!\n");
            break;
    }
}