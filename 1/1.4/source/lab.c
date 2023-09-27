#include "../headers/lab.h"

void count_latin_letters(const char* line, int* counter) {
    *counter = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (isalpha(line[i]) && isascii(line[i])) {
            (*counter)++;
        }
    }
}

void count_non_alpha_numeric(const char* line, int* counter) {
    *counter = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isalnum(line[i]) && !isspace(line[i]) && isascii(line[i])) {
            (*counter)++;
        }
    }
}
