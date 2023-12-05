#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/hashtable.h"

status_code process_file(const char* filename) {
    if (!filename) return code_error_oppening;
    FILE* file = fopen(filename, "r");
    if (!file) {
        fclose(file);
        return code_error_oppening;
    }
    char* line = NULL;
    int read;
    Hash_table* table = NULL;
    status_code st_act;
    st_act = create_table(&table, TABLE_SIZE);
    if (st_act != code_success) {
        fclose(file);
        free_table(table);
        return st_act;
    }
    int index = -1;
    while ((read = getline(&line, &(size_t){0}, file)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        int size_line = strlen(line);
        char* key = (char*)malloc(sizeof(char) * size_line);
        if (!key) {
            fclose(file);
            free_table(table);
            free(key);
            return code_error_alloc;
        }
        char* value = (char*)malloc(sizeof(char) * size_line);
        if (!value) {
            fclose(file);
            free_table(table);
            free(key);
            free(value);
            return code_error_alloc;
        }
        if (sscanf(line, "#define %s %s ", key, value) != 2) {
            free(line);
            line = NULL;
            index = ftell(file);
            break;
        }
        st_act = insert_table(table, key, value);
        if (st_act != code_success) {
            free(line);
            line = NULL;
            fclose(file);
            free_table(table);
            free(key);
            free(value);
            return st_act;
        }
        free(line);
        line = NULL;
        free(key);
        free(value);
        key = NULL;
        value = NULL;
    }
    print_table(table);
    free(line);
    fclose(file);
    return code_success;
}

void print_error(status_code st) {
    switch (st) {
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            printf("Can`t open file!!!\n");
            break;
        default:
            break;
    }
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