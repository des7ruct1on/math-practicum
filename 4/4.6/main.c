#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/tree.h"

void print_error(status_code st, char* expression) {
    switch (st) {
        case code_invalid_parameter:
            fprintf(stdout, "Invalid parameter detected!!!\n");
            fprintf(stdout, "%s\n", expression);
            break;
        case code_error_alloc:
            fprintf(stdout, "Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            fprintf(stdout, "Can`t open file!!!\n");
            break;
        case code_missed_left_bracket:
            fprintf(stdout, "You missed left bracket!!!\n");
            fprintf(stdout, "%s\n", expression);
            break;
        case code_missed_right_bracket:
            fprintf(stdout, "You missed right bracket!!!\n");
            fprintf(stdout, "%s\n", expression);
            break;
        default:
            break;
    }
}

status_code process_file(const char* filename) {
    if (!filename) return code_invalid_parameter;
    FILE* file = fopen(filename, "r");
    if (!file) {
        return code_error_oppening;
    }
    char* line = NULL;
    int read;
    status_code st_act;
    while ((read = getline(&line, &(size_t){0}, file)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }

        char* pol_expression = NULL;
        st_act = to_pol_expression(line, &pol_expression);
        if (st_act != code_success) {
            print_error(st_act, line);
            free(pol_expression);
            pol_expression = NULL;
            continue;
        }
        printf("%s wow\n", pol_expression);
        free(line);
        line = NULL;
    }
    free(line);
    line = NULL;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <FILE>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    status_code st_act;
    st_act = process_file(filename);
    return 0;
}