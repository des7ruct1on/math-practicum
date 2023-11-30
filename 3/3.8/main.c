#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

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
    if (argc < 2) {
        printf("Your programm must start with: %s <FILE_IN>\n", argv[0]);
        return -1;
    }
    status_code st_act = read_from_file(argv[1]);
    print_error(st_act);
    return 0;
}