#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

status_code process_file(const char* filename) {
    if (!filename) return code_invalid_parameter;
    FILE* file = fopen(filename, "r");
    if (!file) {
        return code_error_oppening;
    }
    char* line = NULL;
    int read;
    status_code st_act;
    Storage* st = NULL;
    st_act = create_storage(&st);
    if (st_act != code_success) {
        fclose(file);
        return st_act;
    }
    while ((read = getline(&line, &(size_t){0}, file)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        printf("line: %s\n", line);
        st_act = process_line(line, st);
        if (st_act != code_success) {
            free(line);
            fclose(file);
            free_storage(st);
            return st_act;
        }
        free(line);
        line = NULL;
    }
    free(line);
    fclose(file);
    free_storage(st);
    return code_success;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Your programm must start with: %s <FILE>\n", argv[0]);
    }
    const char* filename = argv[1];
    status_code st_act;
    st_act = process_file(filename);
    print_error(st_act);
    return 0;
}