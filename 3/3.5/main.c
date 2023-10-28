#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

void print_menu() {
    printf("\n|======================================| Hello, user! |======================================|\n");
    printf("| Enter `Sort` to sort storage with students                                                 |\n");
    printf("| Enter `Find <TYPE> <PARAM> ` to find student and print info about him in <OUTPUT_FILE>     |\n");
    printf("|   TYPES: Name, Surname, Id, Group                                                          |\n");
    printf("| Enter `Table` to print table od students                                                   |\n");
    printf("| Enter `Rewrite` to rewrite <INPUT_FILE>                                                    |\n");
    printf("| Enter `EXIT` to exit                                                                       |\n");
    printf("|============================================================================================|\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm should start at least with: %s <FILE_IN>\n", argv[0]);
        return -1;
    }
    const char* file_in = argv[1];
    const char* file_out = argv[2];
    printf("1\n");
    FILE* in = fopen(file_in, "r");
    if (!in) {
        printf("Can`t open file!!!\n");
        return -2;
    }
    bool has_output = false;
    if (file_out != NULL) {
        has_output = true;
    }
    Student* storage = NULL;
    int capacity;
    printf("2\n");
    status_code st_read = read_from_file(in, &storage, &capacity);
    if (st_read == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free(storage);
        fclose(in);
    } else if (st_read == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        free(storage);
        fclose(in);
    }
    printf("3\n");
    bool is_sorted_storage;
    char* arg_one = NULL;
    char* arg_two = NULL;
    unsigned int find_id;
    status_code st_rewrite, st_find, st_print_avg;
    Student* find = NULL;
    while (true) {
        status_cmd st_cmd = command(&arg_one, &arg_two);
        printf("4\n");
        switch (st_cmd) {
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                free(storage);
                fclose(in);
                return -4;
            case cmd_exit:
                printf("you have successfully logged out of the program\n");
                break;
            case cmd_table:
                print_table(storage, capacity);
                break;
            case cmd_sort:
                printf("You successfully sorted the table\n");
                break;
            case cmd_rewrite_file:
                if (!has_output) {
                    printf("You do not have output file, so the res will be displayed in console\n");
                }
                st_rewrite = rewrite_file(file_out, storage, capacity);
                if (st_rewrite == code_error_oppening) {
                    printf("Can`t open %s!!!\n", file_out);
                    return -5;
                }
                break;
            case cmd_print_stud_more_avg:
                if (!has_output) {
                    printf("You do not have output file, so the res will be displayed in console\n");
                }
                st_print_avg = print_stud_bigger_avg(file_out, storage, capacity);
                if (st_print_avg == code_error_oppening) {
                    printf("Can`t open %s!!!\n", file_out);
                    return -6;
                }
                break;
            case cmd_find_id:
                is_sorted_storage = is_sorted(storage, capacity);
                if (is_sorted_storage) {
                    if (sscanf(arg_two, "%d", &find_id) != 1) {
                        printf("Invalid parameter detected!!!\n");
                        free(arg_two);
                        break;
                    }
                    st_find = get_student_id(storage, &find, capacity, find_id);
                    if (find == NULL) {
                        printf("There is no student with this parameter!\n");
                        break;
                    }
                    if (!has_output) {
                        printf("You do not have output file, so the res will be displayed in console\n");
                    } 
                    st_print_avg = print_student_chars(file_out, find);
                    if (st_print_avg == code_error_oppening) {
                        printf("Can`t open %s!!!\n", file_out);
                        free(arg_two);
                        return -7;
                    }
                } else {
                    printf("Firstly, you must sort the table\n");
                }
                break;
            case cmd_find_group:
                st_find = print_students_group(file_out, arg_two, storage, capacity);
                if (st_find == code_error_oppening) {
                    printf("Can`t open %s!!!\n", file_out);
                    free(arg_two);
                    return -8;
                }
            case cmd_find_name:
                is_sorted_storage = is_sorted(storage, capacity);
                if (is_sorted_storage) {
                    st_find = get_student_name(storage, &find, capacity, arg_two);
                    if (find == NULL) {
                        printf("There is no student with this parameter!\n");
                        break;
                    }
                    if (!has_output) {
                        printf("You do not have output file, so the res will be displayed in console\n");
                    } 
                    st_print_avg = print_student_chars(file_out, find);
                    if (st_print_avg == code_error_oppening) {
                        printf("Can`t open %s!!!\n", file_out);
                        free(arg_two);
                        return -8;
                    }
                } else {
                    printf("Firstly, you must sort the table\n");
                }
                break;
            case cmd_find_surname:
                is_sorted_storage = is_sorted(storage, capacity);
                if (is_sorted_storage) {
                    st_find = get_student_surname(storage, &find, capacity, arg_two);
                    if (find == NULL) {
                        printf("There is no student with this parameter!\n");
                        free(arg_two);
                        break;
                    }
                    if (!has_output) {
                        printf("You do not have output file, so the res will be displayed in console\n");
                    } 
                    st_print_avg = print_student_chars(file_out, find);
                    if (st_print_avg == code_error_oppening) {
                        printf("Can`t open %s!!!\n", file_out);
                        free(arg_two);
                        return -8;
                    }
                } else {
                    printf("Firstly, you must sort the table\n");
                }
                break;
            case cmd_invalid_parameter:
                printf("Unknown option, try again\n");
                break;

        }
        free(arg_two);
        if (st_cmd == cmd_exit) {
            break;
        }
    }
    fclose(in);
    free(storage);

    return 0;
}