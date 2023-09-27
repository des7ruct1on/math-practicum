#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc < 2 || argc > 4) {
        printf("Your programm must start with: %s <FLAG> <INPUT_FILE>\n", argv[0]);
        exit(1);
    }
    int num = atoi(argv[2]);
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/' symbol!\n");
        exit(1);
    }
    char* input_name = argv[2];
    char* output_name = NULL;
    char flag;
    if (argv[1][1] == 'n') {
        output_name = argv[3];
        flag = argv[1][2];
        if (argv[3] == NULL) {
            printf("Your programm must include path of the output file!!!\n");
            exit(1);
        }
    } else {
        flag = argv[1][1];
        char *prefix = "out_";
        output_name = (char *)malloc(strlen(input_name) + strlen(prefix) + 1);
        strcpy(output_name, prefix);
        strcat(output_name, input_name);
    }
    FILE* input_file = fopen(input_name, "r");
    if (input_file == NULL) {
        printf("Can`t open file\n");
        exit(1);
    }
    FILE* output_file = fopen(output_name, "w");
    if (output_file == NULL) {
        printf("Can`t open file\n");
        fclose(input_file);
        exit(1);
    }

    char line[STR_SIZE];
    int counter;
    char tmp_output[STR_SIZE];
    while (fgets(line, sizeof(line), input_file)) {
        switch (flag) {
            case 'd':
                for (int i = 0; line[i] != '\0'; i++) {
                    if (!isdigit(line[i])) {
                        fprintf(output_file, " ");
                    }
                } 
                fputc('\n', output_file);
                break;
            case 'i':
                count_latin_letters(line, &counter);
                fprintf(output_file, "%d\n", counter);
                break;
            case 's':
                count_non_alpha_numeric(line, &counter);
                fprintf(output_file, "%d\n", counter);
                break;
            case 'a':
                for (int i = 0; line[i] != '\0'; i++) {
                    if (isspace(line[i])) {
                        fprintf(output_file, " ");
                    } else if (isdigit(line[i])) {
                        fprintf(output_file, "%c", line[i]);
                    } else if (isascii(line[i])) { 
                        fprintf(output_file, "%02X", line[i]);
                    }
                }
                fprintf(output_file, "\n");
                break;
            default:
                printf("Unknown flag!\n");
                break;
        }
    }
    fclose(input_file);
    fclose(output_file);
    if (argc == 4) {
        free(output_name);
    }
    return 0;
}