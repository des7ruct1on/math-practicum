#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers/lab.h"


int main(int argc, char* argv[]) {
    First_in* ins = NULL;
    int size;
    
    status_code st_find = find_first_in(&ins, &size, "zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }
    printf("---\n\n\n");
    st_find = find_first_in(&ins, &size, "   zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }

    st_find = find_first_in(&ins, &size, "z\nx\nc", 4, "input.txt", "input1.txt", "input3.txt", "input5.txt");
    printf("---\n\n\n");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }
    st_find = find_first_in(&ins, &size, "z\tx\tc", 5, "input.txt", "input1.txt", "input3.txt", "input5.txt", "input4.txt");
    printf("---\n\n\n");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }

    st_find = find_first_in(&ins, &size, " ", 2, "input.txt", "input6.txt");
    printf("---\n\n\n");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }
    printf("---\n\n\n");

    st_find = find_first_in(&ins, &size, "11", 2, "input7.txt", "input8.txt");
    switch(st_find) {
        case code_error_malloc:
            printf("Error malloc detected!!!\n");
            exit(1);
        case code_error_open_file:
            printf("Can't open file!\n");
            exit(2);
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            exit(3);
        case code_success:
            print_ins(ins, size);
            free(ins);
    }
    printf("---\n\n\n");

    return 0;
}
