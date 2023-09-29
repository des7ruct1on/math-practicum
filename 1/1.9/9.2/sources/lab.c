#include "../headers/lab.h"

status_code generate_array(int** arr, int size, int a, int b) {
    *arr = (int*)malloc(size * sizeof(int));
    if (*arr == NULL) {
        return code_malloc_error;
    }
    for (int i = 0; i < size; i++) {
        (*arr)[i] = rand() % (b - a + 1) + a;
    }
    return code_succes;
}

void print_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int find_closest_value(int target, int* arr, int size) {
    int closest_value = arr[0];
    int min_difference = abs(target - arr[0]);

    for (int i = 1; i < size; i++) {
        int difference = abs(target - arr[i]);
        if (difference < min_difference) {
            min_difference = difference;
            closest_value = arr[i];
        }
    }

    return closest_value;
}

status_code combine_arrays(int** arr_c, int* arr_a, int* arr_b, int size_a, int size_b) {
    *arr_c = (int*)malloc(size_a * sizeof(int));
    if (*arr_c == NULL) {
        return code_malloc_error;
    }
    for (int i = 0; i < size_a; i++) {
        int closest_value = find_closest_value(arr_a[i], arr_b, size_b);
        (*arr_c)[i] = arr_a[i] + closest_value;
    }

    return code_succes;
}
