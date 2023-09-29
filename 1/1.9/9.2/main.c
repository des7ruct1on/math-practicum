#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "headers/lab.h"

int main() {
    srand(time(NULL));

    // Generate random sizes for arrays A and B
    int size_a = rand() % (10000 - 10 + 1) + 10;
    int size_b = rand() % (10000 - 10 + 1) + 10;

    // Generate dynamic arrays A and B
    int *arr_a, *arr_b, *arr_c;

    switch (generate_array(&arr_a, size_a, -1000, 1000)) {
        case code_succes:
            break;
        case code_malloc_error:
            printf("Error memory allocation detected!!!\n");
            return 1;
    }

    switch (generate_array(&arr_b, size_b, -1000, 1000)) {
        case code_succes:
            break;
        case code_malloc_error:
            printf("Error memory allocation detected!!!\n");
            free(arr_a);
            return 1;
    }

    // print arrays A and B
    printf("Array A: ");
    print_array(arr_a, size_a);
    printf("\n\n\n");

    printf("Array B: ");
    print_array(arr_b, size_b);
    printf("\n\n\n");

    // combine arrays A and B to create array C
    switch (combine_arrays(&arr_c, arr_a, arr_b, size_a, size_b))
    {
    case code_succes:
        break;
    case code_malloc_error:
        printf("Error memory allocation detected!!!\n");
        free(arr_a);
        free(arr_b);
    }
    // print array C
    printf("Array C: ");
    print_array(arr_c, size_a);

    // free allocated memory
    free(arr_a);
    free(arr_b);
    free(arr_c);

    return 0;
}
