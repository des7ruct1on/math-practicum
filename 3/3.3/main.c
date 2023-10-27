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
    if (argc < 4) {
        printf("Your programm should start with: %s <FLAG> <FILE_IN> <FILE_OUT>\n", argv[0]);
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
        fclose(in);
        return -3;
    }
    Employee* storage = NULL;
    int capacity;
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
    free(storage);

    return 0;
}