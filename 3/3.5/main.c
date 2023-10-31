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
    printf("| Enter `Sort <TYPE>` to sort storage with students                                          |\n");
    printf("|   TYPES: Name, Surname, Group, Id                                                          |\n");
    printf("| Enter `Find <TYPE> <PARAM> ` to find student and print info about him in <OUTPUT_FILE>     |\n");
    printf("|   TYPES: Name, Surname, Id, Group                                                          |\n");
    printf("| Enter `Table` to print table od students                                                   |\n");
    printf("| Enter `Rewrite` to rewrite <INPUT_FILE>                                                    |\n");
    printf("| Enter `Avg` to write down students with avg ball > avg ball among the other students       |\n");
    printf("| Enter `Exit` to exit                                                                       |\n");
    printf("|============================================================================================|\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm should start at least with: %s <FILE_IN>\n", argv[0]);
        return -1;
    }
    const char* file_in = argv[1];
    const char* file_out = argv[2];
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
    status_free st_free;
    status_code st_read = read_from_file(in, &storage, &capacity);
    if (st_read == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        st_free = free_storage(storage, capacity);
        fclose(in);
    } else if (st_read == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        st_free = free_storage(storage, capacity);
        fclose(in);
    }
    fclose(in);
    bool is_sorted_storage;
    char* arg_one = NULL;
    char* arg_two = NULL;
    unsigned int find_id;
    status_code st_rewrite, st_find, st_print_avg;
    Student* find = NULL;
    while (true) {
        print_menu();
        status_cmd st_cmd = command(&arg_one, &arg_two);
        switch (st_cmd) {
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                st_free = free_all(1, storage);
                if (st_free == status_free_fail) {
                    printf("Error detected while freeing!!!\n");
                    return -10;
                }
                fclose(in);
                return -4;
            case cmd_exit:
                printf("you have successfully logged out of the program\n");
                break;
            case cmd_table:
                print_table(storage, capacity);
                break;
            case cmd_sort:
                if (!strcmp(arg_one, "Id")) {
                    qsort(storage, capacity, sizeof(Student), compare_students_id);
                } else if (!strcmp(arg_one, "Group")) {
                    qsort(storage, capacity, sizeof(Student), compare_students_group);
                } else if (!strcmp(arg_one, "Surname")) {
                    qsort(storage, capacity, sizeof(Student), compare_students_surname);
                } else if (!strcmp(arg_one, "Name")) {
                    qsort(storage, capacity, sizeof(Student), compare_students_name);
                } else {
                    printf("Unknown flag sorting!\n");
                    free(arg_one);
                    arg_one = NULL;
                    break;
                }
                printf("You successfully sorted the table\n");
                free(arg_one);
                arg_one = NULL;
                break;
            case cmd_rewrite_file:
                if (!has_output) {
                    printf("You do not have output file!!!\n");
                    break;
                }
                in = fopen(file_in, "w");
                if (!in) {
                    printf("Can`t open file!!!\n");
                    return -2;
                }
                st_rewrite = rewrite_file(in, storage, capacity);
                fclose(in);
                break;
            case cmd_print_stud_more_avg:
                if (!has_output) {
                    printf("You do not have output file!!!\n");
                    break;
                }
                st_print_avg = print_stud_bigger_avg(file_out, storage, capacity);
                if (st_print_avg == code_error_oppening) {
                    printf("Can`t open %s!!!\n", file_out);
                    st_free = free_storage(storage, capacity);
                    fclose(in);
                    return -6;
                }
                break;
            case cmd_find:
                if (!strcmp(arg_one, "Id")) {
                    is_sorted_storage = is_sorted(storage, capacity, compare_students_id);
                } else if (!strcmp(arg_one, "Group")) {
                    is_sorted_storage = true;
                } else if (!strcmp(arg_one, "Surname")) {
                    is_sorted_storage = is_sorted(storage, capacity, compare_students_surname);
                } else if (!strcmp(arg_one, "Name")) {
                    is_sorted_storage = is_sorted(storage, capacity, compare_students_name);
                } else {
                    free(arg_one);
                    arg_one = NULL;
                    printf("Error flag detected!!!\n");
                    break;
                }
                if (is_sorted_storage) {
                    if (!strcmp(arg_one, "Id")) {
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
                            printf("You do not have output file\n");
                            break;
                        } 
                        st_print_avg = print_student_chars(file_out, find);
                        if (st_print_avg == code_error_oppening) {
                            printf("Can`t open %s!!!\n", file_out);
                            st_free = free_storage(storage, capacity);
                            free(arg_two);
                            fclose(in);
                            return -7;
                        }
                    } else if (!strcmp(arg_one, "Group")) {
                        st_find = print_students_group(file_out, arg_two, storage, capacity);
                        if (st_find == code_error_oppening) {
                            printf("Can`t open %s!!!\n", file_out);
                            st_free = free_storage(storage, capacity);
                            free(arg_two);
                            arg_two = NULL;
                            fclose(in);
                            return -8;
                        }  
                    } else if (!strcmp(arg_one, "Surname")) {
                        st_find = get_student_surname(storage, &find, capacity, arg_two);
                        if (find == NULL) {
                            printf("There is no student with this parameter!\n");
                            free(arg_two);
                            arg_two = NULL;
                            break;
                        }
                        if (!has_output) {
                            printf("You do not have output file\n");
                            break;
                        } 
                        st_print_avg = print_student_chars(file_out, find);
                        if (st_print_avg == code_error_oppening) {
                            printf("Can`t open %s!!!\n", file_out);
                            st_free = free_storage(storage, capacity);
                            free(arg_two);
                            arg_two = NULL;
                            fclose(in);
                            return -8;
                        }
                    } else if (!strcmp(arg_one, "Name")) {
                        st_find = get_student_name(storage, &find, capacity, arg_two);
                        if (find == NULL) {
                            printf("There is no student with this parameter!\n");
                            break;
                        }
                        if (!has_output) {
                            printf("You do not have output file!\n");
                            break;
                        } 
                        st_print_avg = print_student_chars(file_out, find);
                        if (st_print_avg == code_error_oppening) {
                            printf("Can`t open %s!!!\n", file_out);
                            st_free = free_storage(storage, capacity);
                            free(arg_two);
                            fclose(in);
                            return -8;
                        }
                    }
                } else {
                    printf("Firstly, you must sort the table\n");
                }
                break;
            case cmd_invalid_parameter:
                printf("Unknown option, try again\n");
                break;

        }
        if (!arg_one) {
            free(arg_one);
            arg_one = NULL;
        }
        if (!arg_two) {
            free(arg_two);
            arg_two = NULL;
        }
        if (st_cmd == cmd_exit) {
            break;
        }
    }
    st_free = free_storage(storage, capacity);

    return 0;
}