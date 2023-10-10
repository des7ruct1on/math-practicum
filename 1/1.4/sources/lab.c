#include "../headers/lab.h"

void count_latin_letters(FILE* input, FILE* output_file) {
    int counter = 0;
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (isalpha(symbol) && isascii(symbol)) {
            counter++;
        }
        symbol = fgetc(input);
        if (symbol == '\n' || symbol == EOF) {
            fprintf(output_file, "%d\n", counter);
            counter = 0;
        }
    }
}

void count_non_alpha_numeric(FILE* input, FILE* output_file) {
    int counter = 0;
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (!isalnum(symbol) && !isspace(symbol) && isascii(symbol)) {
            counter++;
        }
        symbol = fgetc(input);
        if (symbol == '\n' || symbol == EOF) {
            fprintf(output_file, "%d\n", counter);
            counter = 0;
        }
    }
}

void remove_digits(FILE* input, FILE* output) {
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (!isdigit(symbol)) {
            fprintf(output, "%c", symbol);
        }
        symbol = fgetc(input);
    }
}

void remove_non_digits_by_ascii(FILE* input, FILE* output) {
    char symbol = fgetc(input);
    while (symbol != EOF) {
        if (symbol == '\n') {
            symbol = fgetc(input);
        }
        if (isspace(symbol)) {
            fprintf(output, " ");
        } else if (isdigit(symbol)) {
            fprintf(output, "%c", symbol);
        } else if (isascii(symbol)) { 
            fprintf(output, "%02X", symbol);
        }
        symbol = fgetc(input);
    }    
}

