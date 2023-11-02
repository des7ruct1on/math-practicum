#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/tree.h"


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Your programm must start with: %s <FILE_IN> <FILE_OUT>\n", argv[0]);
        return -1;
    }
    const char* file_in = argv[1];
    const char* file_out = argv[2];
    FILE* input = fopen(file_in, "r");
    if (!input) {
        printf("Can`t open file: %s!\n", file_in);
        return -2;
    }
    FILE* output = fopen(file_out, "w");
    if (!output) {
        printf("Can`t open file: %s!\n", file_out);
        return -2;
    }
    status_code st_build = scan_print_tree(input, output);
    if (st_build == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        return -3;
    } else if (st_build == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        return -3;
    }
    fclose(input);
    fclose(output);
    return 0;
}