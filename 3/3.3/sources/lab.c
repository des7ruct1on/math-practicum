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


status_code read_from_file(FILE* in, Employee** storage, int* capacity) {
    *capacity = 16;
    (*storage) = (Employee*)malloc(sizeof(Employee) * (*capacity));
    if (*storage == NULL) {
        return code_error_alloc;
    }
    int index_line = 0;
    int index_storage = 0;
    char* line = NULL;
    bool start = true;
    status_realloc st_realloc;
    int read;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        Employee tmp;
        if (!sscanf(line, "%d %s %s %lf", &(tmp.id), tmp.name, tmp.surname, &(tmp.salary)))  {
            free(line);
            return code_invalid_parameter;
        }
        (*storage)[index_storage] = tmp;
        index_storage++;
        if (*capacity - 1 == index_storage) {
            *capacity *= 2;
            st_realloc = my_realloc(storage, *capacity);
            if (st_realloc == status_realloc_fail) {
                status_free st_free = free_all(2, line, *storage);
                if (st_free == status_free_fail) {
                    return code_error_alloc;
                }
                return code_error_alloc;
            }
        }
        free(line);
        line = NULL;
        start = true;
    }
    free(line);
    *capacity = index_storage;
    return code_success;
}

int compare_employees(const void* a, const void* b) {
    const double epsilon = 1e-10;
    double diff_salary = ((Employee *)a)->salary - ((Employee *)b)->salary;
    if (diff_salary > epsilon) {
        return 1;
    } else if (diff_salary < epsilon) {
        return -1;
    }
    int surname_diff = strcmp(((Employee *)a)->surname, ((Employee *)b)->surname);
    if (surname_diff != 0) {
        return surname_diff;
    }
    int name_diff = strcmp(((Employee *)a)->name, ((Employee *)b)->name);
    if (name_diff != 0) {
        return name_diff;
    }
    return (((Employee *)a)->id - ((Employee *)b)->id);
}

void write_file(FILE* out, Employee* storage, int size, Type_sort order_status) {
    if (order_status == ASCENDING) {
        for (int i = 0; i < size; i++) {
            Employee tmp = storage[i];
            fprintf(out, "%d %s %s %lf", tmp.id, tmp.name, tmp.surname, tmp.salary);
            fprintf(out, "\n");
        }
    } else {
        for (int i = size - 1; i >= 0; i--) {
            Employee tmp = storage[i];
            fprintf(out, "%d %s %s %lf", tmp.id, tmp.name, tmp.surname, tmp.salary);
            fprintf(out, "\n");
        }
    }
}