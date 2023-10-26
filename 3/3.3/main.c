#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

int read_str(const char* str, Employee* tmp) {
    
}

status_code read_from_file(FILE* in, Employee** storage, int* capacity) {
    *capacity = 16;
    printf("0\n");
    (*storage) = (Employee*)malloc(sizeof(Employee) * (*capacity));
    if (*storage == NULL) {
        return code_error_alloc;
    }
    char c;
    printf("0\n");
    int index_line = 0;
    int index_storage = 0;
    char* line = NULL;
    bool start = true;
    status_realloc st_realloc;
    bool is_feof = feof(in);
    while (!is_feof) {
        printf("1\n");
        c = fgetc(in);
        if (c == EOF) {
            break;  // Exit the loop when reaching the end of the file
        }
        int line_size = 32;
        line = malloc(sizeof(char) * line_size);
        if (!line) {
            return code_error_alloc;
        }
        while(c != '\n' || start) {
            line[index_line] = c;
            printf("    %c -- %d\n", line[index_line], index_line);
            index_line++;
            c = fgetc(in);
            start = false;
            if (line_size - 1 == index_line) {
                line_size *= 2;
                st_realloc = my_realloc(storage, line_size);
                if (st_realloc == status_realloc_fail) {
                    status_free st_free = free_all(2, line, *storage);
                    if (st_free == status_free_fail) {
                        return code_error_alloc;
                    }
                    return code_error_alloc;
                }
            }
        }
        line[index_line] = '\0';
        is_feof = feof(in);
        printf("2\n");
        Employee tmp;
        if (!sscanf(line, "%d %s %s %lf", &(tmp.id), tmp.name, tmp.surname, &(tmp.salary)))  {
            free(line);
            return code_invalid_parameter;
        }
        printf("%d %s %s %lf\n", tmp.id, tmp.name, tmp.surname, tmp.salary);
        printf("3\n");
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
        printf("4\n");
        free(line);
        line = NULL;
        start = true;
    }
    *capacity = index_storage;
    return code_success;
}

typedef enum Type_sort {
    ASCENDING,
    DESCENDING
} Type_sort;


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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Your programm should start with: %s <FILE_IN> <FLAG> <FILE_OUT>\n", argv[0]);
        return -1;
    }
    char* flag = argv[1];
    const char* file_in = argv[2];
    const char* file_out = argv[3];
    FILE* in = fopen(file_in, "r");
    if (!in) {
        printf("Can`t open file!!!\n");
        return -2;
    }
    FILE* out = fopen(file_out, "w");
    if (!out) {
        printf("Can`t open file!!!\n");
        return -3;
    }
    Employee* storage = NULL;
    int capacity;
    printf("sdfsd\n");
    status_code st_read = read_from_file(in, &storage, &capacity);
    if (st_read == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free(storage);
        fclose(in);
        fclose(out);
    } else if (st_read == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        free(storage);
        fclose(in);
        fclose(out);
    }
    printf("e231\n");
    Type_sort st_sort;
    if (!strcmp(flag, "-d") || !strcmp(flag, "/d")) {
        st_sort = DESCENDING;
    } else if (!strcmp(flag, "-a") || !strcmp(flag, "/a")) {
        st_sort = ASCENDING;
    } else {
        printf("Incorrect flag!!!\n");
        return -4;
    }
    qsort(storage, capacity, sizeof(Employee), compare_employees);
    write_file(out, storage, capacity, st_sort);
    fclose(in);
    fclose(out);

    return 0;
}