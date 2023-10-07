#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        printf("Your programm must start at least with: %s <FLAG> <FILE_1> <FILE_OUT>\n", argv[0]);
        exit(1);
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/' symbol!\n");
        exit(1);
    }
    char* input_name1 = argv[2];
    char* input_name2 = NULL;
    char* output_name = NULL;
    char flag = argv[1][1];
    FILE* input_file2;
    if (flag == 'r') {
        if (argc != 5) {
            printf("Your programm must start with: %s <FLAG> <FILE_1> <FILE_2> <FILE_OUT>\n", argv[0]);
            exit(1);
        }
        input_name2 = argv[3];
        input_file2 = fopen(input_name2, "r");
        if (input_file2 == NULL) {
            printf("Can`t open file\n");
            exit(1);
        }
        output_name = argv[4];
        if (argv[3] == NULL) {
            printf("Your programm must include path of the output file!!!\n");
            exit(1);
        }
    }
    else if (flag == 'a') {
        if (argc != 4) {
            printf("Your programm must start with: %s <FLAG> <FILE_IN> <FILE_OUT>\n", argv[0]);
            exit(1);
        }
        output_name = argv[3];
    }
    FILE* output_file = fopen(output_name, "w");
    if (output_file == NULL) {
        if (!strcmp(output_name, argv[4])) {
            fclose(input_file2);
        }
        printf("Can`t open file\n");
        exit(1);
    }

    FILE* input_file1 = fopen(input_name1, "r");
    if (input_file1 == NULL) {
        printf("Can`t open file\n");
        exit(1);
    }
    switch (flag) {
        case 'r':
            rewrite_by_two_files(input_file1, input_file2, output_file);
            fclose(input_file2);
            break;
        case 'a':
            generate_lexems(input_file1, output_file);
            break;
        default:
            printf("Unknown flag!\n");
            break;
    }
    fclose(input_file1);
    fclose(output_file);
    return 0;
}