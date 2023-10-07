#include "../headers/lab.h"

void generate_lexems(FILE* input_file, FILE* output_file) {
    char lexem[STR_SIZE];
    int lexeme_counter = 1;
    while (fscanf(input_file, "%s", lexem) != EOF) {
        int length = strlen(lexem);
        if (lexeme_counter % 10 == 0) {
            for (int i = 0; i < length; i++) {
                printf("%c", tolower(lexem[i]));
                if (isalpha(lexem[i])) {
                    int new_symb_ascii = toascii(tolower(lexem[i]));
                    int new_num = 0;
                    int k = 1;
                    while (new_symb_ascii > 0) {
                        int remainder = new_symb_ascii % 4;
                        new_num += remainder * k;
                        new_symb_ascii /= 4;
                        k *= 10;
                    }
                    fprintf(output_file, "%d", new_num);
                } else {
                    fprintf(output_file, "%c", lexem[i]);
                }
            }
        } else if (lexeme_counter % 5 == 0 && lexeme_counter % 10 != 0) {
            for (int i = 0; i < length; i++) {
                int new_symb_ascii = toascii(lexem[i]);
                int new_num = 0;
                int k = 1;
                while (new_symb_ascii > 0) {
                    int remainder = new_symb_ascii % 8;
                    new_num += remainder * k;
                    new_symb_ascii /= 8;
                    k *= 10;
                }
                fprintf(output_file, "%d", new_num);
            }
        } else if (lexeme_counter % 2 == 0 && lexeme_counter % 10 != 0) {
            for (int i = 0; i < length; i++) {
                fprintf(output_file, "%c", tolower(lexem[i]));
            }
        } else {
            fprintf(output_file, "%s", lexem);
        }
        fprintf(output_file, " ");
        lexeme_counter++;
    }
}

void rewrite_by_two_files(FILE* input_file1, FILE* input_file2, FILE* output_file) {
    char line_1[STR_SIZE];
    char line_2[STR_SIZE];
    bool file_1_ended = false;
    bool file_2_ended = false;

    while (true) {
        if (!file_1_ended && fscanf(input_file1, "%s", line_1) == 1) {
            fprintf(output_file, "%s ", line_1);
        } else {
            file_1_ended = true;
        }

        if (!file_2_ended && fscanf(input_file2, "%s", line_2) == 1) {
            fprintf(output_file, "%s ", line_2);
        } else {
            file_2_ended = true;
        }

        if (file_1_ended && file_2_ended) {
            break;
        }
    }
}
