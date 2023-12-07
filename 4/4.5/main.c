#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

void print_error(status_code st, char* expression, FILE* out_file, int index) {
    switch (st) {
        case code_invalid_parameter:
            fprintf(out_file, "Invalid parameter detected!!!\n");
            fprintf(out_file, "%d %s\n", index, expression);
            break;
        case code_error_alloc:
            fprintf(out_file, "Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            fprintf(out_file, "Can`t open file!!!\n");
            break;
        case code_missed_left_bracket:
            fprintf(out_file, "You missed left bracket!!!\n");
            fprintf(out_file, "%d %s\n", index, expression);
            break;
        case code_missed_right_bracket:
            fprintf(out_file, "You missed right bracket!!!\n");
            fprintf(out_file, "%d %s\n", index, expression);
            break;
        default:
            break;
    }
}

void print_info(char* expr, char* postfix, long long res) {
    if (!expr) return;
    printf("|=============================================|\n");
    printf("| expression | %s\n", expr);
    printf("|   result   | %lld\n", res);
    printf("|   postfix  | ");
    int size = strlen(postfix);
    for (int i = 0; i < size; i++) {
        if (postfix[i] != ',') {
            printf("%c", postfix[i]);
        }
    }
    printf("\n");
    printf("|=============================================|\n");
}

status_code process_files(char* argv[], int argc) {
    if (argc < 2) return code_invalid_parameter;

    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        //printf("wow %s\n", filename);
        if (!filename) return code_invalid_parameter;
        FILE* file = fopen(filename, "r");
        if (!file) {
            return code_error_oppening;
        }
        char* tmp_filename = NULL;
        tmp_filename = strdup("tmp_");
        strcat(tmp_filename, filename);
        if (!tmp_filename) {
            fclose(file);
            return code_invalid_parameter;
        }
        FILE* file_tmp = fopen(tmp_filename, "a+");
        if (!file) {
            fclose(file);
            return code_error_oppening;
        }
        char* line = NULL;
        int read;
        int index = 0;
        status_code st_act;
        while ((read = getline(&line, &(size_t){0}, file)) != -1) {
            if (read == -1) {
                free(line);
                return code_error_alloc;
            }
            //printf("%s\n", line);
            long long res;
            char* pol_expression = NULL;
            st_act = to_pol_expression(line, &pol_expression);
            if (st_act != code_success) {
                index++;
                print_error(st_act, line, file_tmp, index);
                free(pol_expression);
                pol_expression = NULL;
                continue;
            }
            st_act = compute(pol_expression, &res);
            if (st_act != code_success) {
                print_error(st_act, line, file_tmp, index);
                free(pol_expression);
                pol_expression = NULL;
                index++;
                continue;
            }
            index++;
            print_info(line, pol_expression, res);
            free(line);
            free(pol_expression);
            pol_expression = NULL;
            line = NULL;
        }
        free(line);
        line = NULL;
        free(tmp_filename);
        fclose(file);
        fclose(file_tmp);
    }
    return code_success;
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <FILE>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    status_code st_act;
    st_act = process_files(argv, argc);
    return 0;
}