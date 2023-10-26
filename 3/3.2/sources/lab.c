#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}

status_code get_euclidean_norm(double* res, Vector* vector, int dimension) {
    *res = 0;
    for (int i = 0; i < dimension; i++) {
        *res += vector->coordinates[i] * vector->coordinates[i];
    }
    *res = sqrt(*res);
    return code_success;
}

void print_vector(Vector* arr, int size) {
    printf("<");
    for (int i = 0; i < size; i++) {
        printf("%.3lf", arr->coordinates[i]);
        if (i != size - 1) {
            printf(", ");
        }
    }
    printf(">");
    printf("\n");
}


status_code get_max_len_vector(Vector** max_vectors, int* size, int dimension, int count,...) {
    if (count < 0 || dimension < 1) {
        return code_invalid_parameter;
    }
    *size = count;
    const double epsilon = 1e-10;
    va_list ptr;
    (*max_vectors) = (Vector*)malloc(sizeof(Vector) * count);
    if (!(*max_vectors)) {
        return code_error_alloc;
    }
    va_start(ptr, count);
    Vector first = va_arg(ptr, Vector);
    //print_vector(&first, dimension);
    double max = 0.0;
    status_code st_norm = get_euclidean_norm(&max, &first, dimension);
    int index_max = 0;
    //printf("len = %lf\n", max);
    for (int i = 1; i < count; i++) {
        Vector tmp = va_arg(ptr, Vector);
        //print_vector(&tmp, dimension);
        double len_tmp;
        st_norm = get_euclidean_norm(&len_tmp, &tmp, dimension);
        //printf("len = %lf\n", len_tmp);
        if (len_tmp - max > epsilon) {
            max = len_tmp;
            index_max = 1;
            (*max_vectors)[0] = tmp; 
        } else if (fabs(len_tmp - max) < epsilon) {
            (*max_vectors)[index_max++] = tmp; 
        }
    }
    *size = index_max;
    va_end(ptr);
    return code_success;
}