#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    int numerator, denumerator;
    double* res = NULL;
    int count = 5;
    switch (check_finite(&res, 16, count, 0.5, 0.25, 0.39453125, 0.45703125, 0.4)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            free(res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            free(res);
            break;
        case code_success:
            print_arr(res, count);
            free(res);
            break;
    }
    res = NULL;
    switch (check_finite(&res, 10, count, 0.5, 0.25, 0.1, 0.9, 0.4)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            free(res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            free(res);
            break;
        case code_success:
            print_arr(res, count);
            free(res);
            break;
    }
    res = NULL;
    switch (check_finite(&res, 5, count, 0.5, 0.25, 0.1, 0.9, 0.4)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            free(res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            free(res);
            break;
        case code_success:
            print_arr(res, count);
            free(res);
            break;
    }
    return 0;
}