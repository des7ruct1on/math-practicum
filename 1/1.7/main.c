#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#define STR_SIZE 256
//#include "headers/lab.h"

void generate_lexems(FILE* input_file, FILE* output_file) {
    char line[STR_SIZE];
    int lexeme_counter = 0;
    while (fgets(line, sizeof(line), input_file)) {
        char* token = strtok(line, " \t\n"); // Разбиваем строку на лексемы
        while (token != NULL) {
            if (lexeme_counter % 10 == 0) {
                char* first_index = token;
                while (*first_index) {
                    if (isalpha(*first_index)) {
                        // Преобразуем буквы в строчные и потом в ASCII с основанием 4
                        fprintf(output_file, "%o ", tolower(*first_index));
                    } else {
                        fprintf(output_file, "%c", *first_index);
                    }
                    first_index++;
                }
            } else if (lexeme_counter % 5 == 0 && lexeme_counter % 10 != 0) {
                char* first_index = token;
                while (*first_index) {
                    // Преобразуем символы в ASCII с основанием 8
                    fprintf(output_file, "%o ", *first_index);
                    first_index++;
                }
            } else if (lexeme_counter % 2 == 0 && lexeme_counter % 10 != 0) {
                char* first_index = token;
                while (*first_index) {
                    // Преобразуем буквы в строчные буквы
                    fprintf(output_file, "%c", tolower(*first_index));
                    first_index++;
                }
            } else {
                fprintf(output_file, "%s", token);
            }
            token = strtok(NULL, " \t\n"); // Получаем следующую лексему
            if (token != NULL) {
                fprintf(output_file, " "); // Разделяем лексемы пробелами
            }
            lexeme_counter++;
        }
    }
}

void rewrite_by_two_files(FILE* input_file1, FILE* input_file2, FILE* output_file) {
    char line_1[STR_SIZE];
    char line_2[STR_SIZE];
    bool file_1_ended = false;
    bool file_2_ended = false;
    while (true) {
        if (file_1_ended && file_2_ended) {
            break;
        }
        if (!file_1_ended) {
            fscanf(input_file1, "%s", line_1);
            fprintf(output_file, "%s ", line_1);
            if (feof(input_file1)) {
                file_1_ended = true;
            }
        }

        if (!file_2_ended) {
            fscanf(input_file2, "%s", line_2);
            fprintf(output_file, "%s ", line_2);
            if (feof(input_file2)) {  
                file_2_ended = true;
            }
        }
    }
}

int main(int argc, const char *argv[]) {
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
    } else if (flag == 'a') { 
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

    char line_1[STR_SIZE];
    char line_2[STR_SIZE];
    bool file_1_ended = false;
    bool file_2_ended = false;
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