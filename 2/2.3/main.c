#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {

    status_code st_find = find_first_in("zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");

    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }
    printf("\n\n\n");
    st_find = find_first_in("   zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");
    printf("\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }

    st_find = find_first_in("z\nx\nc", 4, "input.txt", "input1.txt", "input3.txt", "input5.txt");
    printf("\n\n\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }

    st_find = find_first_in("z\tx\tc", 5, "input.txt", "input1.txt", "input3.txt", "input5.txt", "input4.txt");
    printf("\n\n\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }
    printf("\n\n\n");

    st_find = find_first_in("\t\t\t", 2, "input.txt", "input6.txt");
    printf("\n\n\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }
    printf("\n\n\n");


    return 0;
}
