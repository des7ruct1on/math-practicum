#include "../headers/lab.h"

int convert_from_decimal(int num, int base) {
    int number = 0, k = 1;
    while (num > 0) {
        int remainder = num % base;
        number += remainder * k;
        num /= base;
        k *= 10;
    }
    return number;
}

void generate_lexems(FILE* input_file, FILE* output_file) {
    char symbol = fgetc(input_file);
    int lexeme_counter = 1;
    while (symbol != EOF) {
        while(isspace(symbol)) {
            symbol = fgetc(input_file);
        }
        if (lexeme_counter % 10 == 0) {
            while (!isspace(symbol) && symbol != EOF) {
                if (isalpha(symbol)) {
                    int digit = toascii(tolower(symbol));
                    fprintf(output_file, "%d", convert_from_decimal(digit, 4));
                } else {
                    fprintf(output_file, "%c", symbol);
                }
                symbol = fgetc(input_file);
            }
        } else if (lexeme_counter % 5 == 0 && lexeme_counter % 10 != 0) {
            while (!isspace(symbol) && symbol != EOF) {
                int digit = toascii(symbol);
                fprintf(output_file, "%d", convert_from_decimal(digit, 8));
                symbol = fgetc(input_file);
            }
        } else if (lexeme_counter % 2 == 0 && lexeme_counter % 10 != 0) {
            while (!isspace(symbol) && symbol != EOF) {
                if (isalpha(symbol)) {
                    fprintf(output_file, "%c", tolower(symbol));
                } else {
                    fprintf(output_file, "%c", symbol);
                }  
                symbol = fgetc(input_file);
            }
            
        } else {
            while(!isspace(symbol)) {
                fprintf(output_file, "%c", symbol);
                symbol = fgetc(input_file);
            }
        }
        fprintf(output_file, " ");
        lexeme_counter++;
    }
}

void rewrite_by_two_files(FILE* input_file1, FILE* input_file2, FILE* output_file) {
    char symb_file1 = fgetc(input_file1);
    char symb_file2 = fgetc(input_file2);
    bool is_feof1 = feof(input_file1);
    bool is_feof2 = feof(input_file1);
    while (true) {
        while(isspace(symb_file1)) {
            symb_file1 = fgetc(input_file1);
        }
        while(isspace(symb_file2)) {
            symb_file2 = fgetc(input_file2);
        }
        while (!isspace(symb_file1) && symb_file1 != EOF) {
            if (!is_feof2 && feof(input_file2)) {
                fprintf(output_file, " ");
                is_feof2 = true;
            }
            fprintf(output_file, "%c", symb_file1);
            symb_file1 = fgetc(input_file1);
        }
        if (symb_file1 != EOF) {
            fprintf(output_file, " ");
        }
        while (!isspace(symb_file2) && symb_file2 != EOF) {
            if (!is_feof1 && feof(input_file1)) {
                is_feof1 = true;
                fprintf(output_file, " ");
            }
            fprintf(output_file, "%c", symb_file2);
            symb_file2 = fgetc(input_file2);
        }
        if (symb_file2 != EOF) {
            fprintf(output_file, " ");
        } 
        if (symb_file1 == EOF && symb_file2 == EOF) {
            break;
        }
    }
}
