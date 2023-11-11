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
    printf("| Enter `Add <INFO>` to add citizen                                                          |\n");
    printf("| Enter `File <INFO>` to add citizenz from file                                              |\n");
    printf("| Enter `Remove <INFO>` to remove citizen                                                    |\n");
    printf("| Enter `Find <Surname Name Last_name>` to add citizen                                       |\n");
    printf("| Enter `Edit <Type> <Info> <Find info>` to edit info about citizen                          |\n");
    printf("|   Type: surname, name, last_name, birthday, gender, salary                                 |\n");
    printf("| Enter `Undo` to undo last N/2 operations                                                   |\n");
    printf("| Enter `Print <filename>` to print citizens in file                                         |\n");
    printf("| Enter `Exit` to exit                                                                       |\n");
    printf("|============================================================================================|\n");
}

int main(int argc, char* argv[]) {
    status_code st_act;
    List_node* storage = NULL;
    Stack* actions = NULL;
    status_cmd decision;
    char* arg_one = NULL;
    char* arg_two = NULL;
    char* arg_three = NULL;
    My_string* info = NULL;
    My_string* find_key = NULL;
    int index_find;
    Action* tmp = NULL;
    while (true) {
        Liver* tmp_liver = NULL;
        print_menu();
        decision = command(&arg_one, &arg_two, &arg_three, &info);
        if (!actions) {
            st_act = stack_init(&actions);
            if (st_act == code_error_alloc) {
                printf("Error alloc detected!\n");
                return 1;
            }
        }
        switch (decision) {
            case cmd_file:
                st_act = read_from_file(info->data, &storage, actions);
                if (st_act == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } 
                if (st_act == code_error_oppening) {
                    printf("Can`t open file!!!\n");
                } 
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } 
                break;
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            case cmd_add:
                st_act = create_liver(info->data, &tmp_liver);
                if (st_act == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else {
                    st_act = push_list(&storage, tmp_liver, actions);
                    if (st_act == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    }
                }
                break;
            case cmd_remove:
                st_act = remove_list(&storage, info, actions);
                if (st_act == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } 
                break;
            case cmd_find:
                st_act = find_citizen(storage, info, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    print_liver(tmp_liver);
                }
                break;
            case cmd_edit_surname:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_surname(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                }
                if (arg_three) free(arg_three); arg_three = NULL;
                break;
            case cmd_edit_name:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_name(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                }
                if (arg_three) free(arg_three); arg_three = NULL;
                break;
            case cmd_edit_last_name:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_last_name(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                }
                if (arg_three) free(arg_three);
                break;
            case cmd_edit_date_birth:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_date_birth(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                    st_act = remove_list(&storage, find_key, actions);
                    tmp = stack_pop(actions);
                    st_act = push_list(&storage, tmp->condition, actions);
                    tmp = stack_pop(actions);
                }
                if (arg_three) free(arg_three); arg_three = NULL;
                break;
            case cmd_edit_gender:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_gender(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                }
                if (arg_three) free(arg_three); arg_three = NULL;
                break;
            case cmd_edit_salary:
                find_key = String(arg_three);
                st_act = find_citizen(storage, find_key, &tmp_liver, &index_find);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_act = edit_salary(tmp_liver, info, index_find, actions);
                    if (st_act == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    } 
                }
                if (arg_three) free(arg_three); arg_three = NULL;
                break;
            case cmd_print:
                st_act = print_file(info->data, storage);
                if (st_act == code_error_oppening) {
                    printf("Can`t open file!!!\n");
                }
                break;
            case cmd_undo:
                st_act = undo(storage, actions);
                if (st_act == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                //stack_destroy(actions);
                actions = NULL;
                break;
            case cmd_exit:
                break;
            default:
                break;
        }
        string_clear(info);
        free(info);
        if (arg_two) free(arg_two); arg_two = NULL;
        if (find_key) string_clear(find_key); free(find_key); find_key = NULL;
        info = NULL;
        if (decision == cmd_error_alloc) {
            break;
        }
        if (st_act == code_error_alloc) {
            break;
        }
        if (decision == cmd_exit) {
            break;
        }
    }
    stack_destroy(actions);
    free_list(storage);
    free(actions);
    return 0;
}