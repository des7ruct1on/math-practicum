#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    Vector* maxs = NULL;
    Vector vector1, vector2, vector3, vector4, vector5;
    int dimension = 3;
    vector1.coordinates = (double*)malloc(dimension * sizeof(double));
    vector1.coordinates[0] = 1.0;
    vector1.coordinates[1] = 2.0;
    vector1.coordinates[2] = 3.0;

    vector2.coordinates = (double*)malloc(dimension * sizeof(double));
    vector2.coordinates[0] = 24.0;
    vector2.coordinates[1] = 0.0;
    vector2.coordinates[2] = 1.0;

    vector3.coordinates = (double*)malloc(dimension * sizeof(double));
    vector3.coordinates[0] = 0.0;
    vector3.coordinates[1] = 0.0;
    vector3.coordinates[2] = 0.0;

    vector4.coordinates = (double*)malloc(dimension * sizeof(double));
    vector4.coordinates[0] = 24.0;
    vector4.coordinates[1] = 3.0;
    vector4.coordinates[2] = 3.0;

    vector5.coordinates = (double*)malloc(dimension * sizeof(double));
    vector5.coordinates[0] = 15.0;
    vector5.coordinates[1] = -7.0;
    vector5.coordinates[2] = 23.0;
    int count = 5;
    int size;
    int p = 1;
    status_code st_find = get_max_len_vector(&maxs, &size, get_matrix_norm, dimension, p, count, vector1, vector2, vector3, vector4, vector5);
    if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free_all(6, vector1.coordinates, vector2.coordinates, vector3.coordinates, vector4.coordinates, vector5.coordinates, maxs);
        return -2;
    }
    for (int i = 0; i < size; i++) {
        Vector tmp = maxs[i];
        print_vector(&tmp, dimension);
    }
    free_all(6, vector1.coordinates, vector2.coordinates, vector3.coordinates, vector4.coordinates, vector5.coordinates, maxs);
    return 0;

}