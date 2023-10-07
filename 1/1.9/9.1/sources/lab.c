#include "../headers/lab.h"

void fill_array(int arr[], int size, int a, int b) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = rand() % (b - a + 1) + a;
    }
}

void print_array(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap_min_max(int arr[], int size) {
    if (size <= 1) {
        return;
    }

    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_index]) {
            min_index = i;
        }
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }

    int temp = arr[min_index];
    arr[min_index] = arr[max_index];
    arr[max_index] = temp;
}
