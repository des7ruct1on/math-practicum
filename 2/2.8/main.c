#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {

    char* res = NULL;

    switch (sum_numbers_by_base(16, &res, 2, "AFB", "FBCAFC")) {
        case code_success:
            printf("Result: %s\n", res);
            free_all(1, res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_alloc_fail:
            printf("Error alloc detected!!!\n");
            break;
    }
    printf("\n");

    switch (sum_numbers_by_base(16, &res, 5,"FBCAFC", "AFB", "A", "F55CBACF", "FFFFFF")) {
        case code_success:
            printf("Result: %s\n", res);
            free_all(1, res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_alloc_fail:
            printf("Error alloc detected!!!\n");
            break;
    }
    printf("\n");

    switch (sum_numbers_by_base(10, &res, 4, "123", "9999", "1", "2344")) {
        case code_success:
            printf("Result: %s\n", res);
            free_all(1, res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_alloc_fail:
            printf("Error alloc detected!!!\n");
            break;
    }
    printf("\n");

    switch (sum_numbers_by_base(36, &res, 4, "ZZZ", "ABZC", "H8", "0")) {
        case code_success:
            printf("Result: %s\n", res);
            free_all(1, res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_alloc_fail:
            printf("Error alloc detected!!!\n");
            break;
    }
    printf("\n");
    switch (sum_numbers_by_base(10, &res, 1, "123")) {
        case code_success:
            printf("Result: %s\n", res);
            free_all(1, res);
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_alloc_fail:
            printf("Error alloc detected!!!\n");
            break;
    }
    printf("\n");


    return 0;
}