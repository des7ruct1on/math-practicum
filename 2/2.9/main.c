#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    int numerator, denumerator;
    double* res = NULL;
    int size;
    switch (check_finite(&res, 16, &size, 5, 0.5, 0.25, 0.39453125, 0.45703125, 0.4)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_success:
            print_arr(res, size);
            break;
    }
    free(res);
    res = NULL;
    switch (check_finite(&res, 10, &size, 9, 0.5, 0.25, 0.1, 0.9, 0.4, 0.0, 0.99, 0.35654, 0.01)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_success:
            print_arr(res, size);
            break;
    }
    free(res);
    res = NULL;
    switch (check_finite(&res, 5, &size, 7, 0.5, 0.25, 0.1, 0.9, 0.4, 0.2, 0.8)) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_success:
            print_arr(res, size);
            break;
    }
    free(res);
    res = NULL;
    return 0;
}