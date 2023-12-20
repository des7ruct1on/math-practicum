#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/hashtable.h"

#define macro 1      + 2

status_code process_file(const char* filename) {
    //printf("%d\n", macro);
    if (!filename) return code_error_oppening;
    FILE* file = fopen(filename, "r");
    if (!file) {
        fclose(file);
        return code_error_oppening;
    }
    char* tmp_filename = NULL;
    tmp_filename = strdup("tmp_");
    strcat(tmp_filename, filename);
    if (!tmp_filename) {
        fclose(file);
        return code_invalid_parameter;
    }
    FILE* file_tmp = fopen(tmp_filename, "w");
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
    while ((read = getline(&line, &(size_t){0}, file)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        //printf("%s line\n", line);
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
        int ch = sscanf(line, "#define %s %[^\n]", key, value);
        //printf("%d\n", ch);
        if (ch != 2) {
            
            if (!strcmp(line, "\0") || !strcmp(line, "\n") || !strcmp(line, "\r\n")) {
                fprintf(file_tmp, "%s", line);
            }
            free(line);
            line = NULL;
            if (key) free(key);
            if (value) free(value);
            break;
        }
        st_act = insert_table(&table, key, value);
        if (st_act != code_success) {
            free(line);
            line = NULL;
            fclose(file);
            free_table(table);
            if (key) free(key);
            if (value) free(value);
            return st_act;
        }
        fprintf(file_tmp, "%s", line);
        free(line);
        line = NULL;
        key = NULL;
        value = NULL;
        //printf("NOWWW\n");
        //print_table(table);
        //printf("    %d>\n", table->size);
        //printf("`~~~~~~NOWWW\n");
    }
    free(line);
    char c = fgetc(file);
    int capacity = 256;
    int index = 0;
    char* word = (char*)malloc(sizeof(char) * capacity);
    if (!word) {
        fclose(file);
        fclose(file_tmp);
        free_table(table);
        free(word);
        return code_error_alloc;
    }
    bool word_scanned = false;
    //printf("`~~~~~~NOWWW\n");

    //printf("symb %c\n", c);
    while (c != EOF) {
        //printf("symb %c\n", c);
        if (!isspace(c) && c != EOF) {
            word[index] = c;
            //printf("symb %c\n", c);
            index++;
            word_scanned = true;
        } else {
            if (word_scanned) {
                word[index] = '\0';
                index = 0;
                //printf("%s word\n", word);
                char* to_replace = find_element(table, word);
                //printf("%s--------\n", to_replace);
                if (!to_replace) {
                    fprintf(file_tmp, "%s", word);
                    fprintf(file_tmp, "%c", c);
                } else {
                    fprintf(file_tmp, "%s", to_replace);
                    fprintf(file_tmp, "%c", c);
                }
                free(word);
                word = NULL;
                word = (char*)malloc(sizeof(char) * capacity);
                if (!word) {
                    fclose(file);
                    fclose(file_tmp);
                    free_table(table);
                    free(word);
                    return code_error_alloc;
                }
                word_scanned = false;
            } else {
                fprintf(file_tmp, "%c", c);
            }


        }

        c = fgetc(file);
    }
    if (word_scanned) {
        word[index] = '\0';
        index = 0;
        char* to_replace = find_element(table, word);
        //printf("%s--------\n", to_replace);
        if (!to_replace) {
            fprintf(file_tmp, "%s", word);
        } else {
            fprintf(file_tmp, "%s", to_replace);
        }
        free(word);
        word = NULL;
        word_scanned = false;
    }
    if (word) free(word);
    if (rename(tmp_filename, filename)) {
        fclose(file);
        fclose(file_tmp);
        free_table(table);
        return code_error_oppening;
    }
    fclose(file);
    fclose(file_tmp);
    free_table(table);
    free(tmp_filename);
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