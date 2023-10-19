#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headers/lab.h"

int main() {
    srand(time(NULL));

    int size_a = rand() % 9991 + 10;
    int size_b = rand() % (10000 - 10 + 1) + 10;

    int *arr_a, *arr_b, *arr_c;

    switch (generate_array(&arr_a, size_a, -1000, 1000)) {
        case code_succes:
            break;
        case code_malloc_error:
            printf("Error memory allocation detected!!!\n");
            exit(1);
    }

    switch (generate_array(&arr_b, size_b, -1000, 1000)) {
        case code_succes:
            break;
        case code_malloc_error:
            printf("Error memory allocation detected!!!\n");
            free(arr_a);
            exit(1);
    }

    printf("Array A: ");
    print_array(arr_a, size_a);
    printf("\n\n\n");

    printf("Array B: ");
    print_array(arr_b, size_b);
    printf("\n\n\n");

    switch (combine_arrays(&arr_c, arr_a, arr_b, size_a, size_b))
    {
    case code_succes:
        break;
    case code_malloc_error:
        printf("Error memory allocation detected!!!\n");
        free(arr_a);
        free(arr_b);
        exit(1);
    }
    printf("Array C: ");
    print_array(arr_c, size_a);
    free(arr_a);
    free(arr_b);
    free(arr_c);

    return 0;
}
