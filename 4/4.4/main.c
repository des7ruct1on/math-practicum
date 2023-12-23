#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

void print_error(status_code st_act) {
    switch (st_act) {
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            printf("Can`t open file!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter!!!\n");
            break;
        case code_success:
            break;
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        printf("Invalid parameter detected!!!\n");
        exit(1);
    }
    status_code st_act;
    Vector** storage = NULL;
    storage = (Vector**)malloc(sizeof(Vector) * 32);
    if (!storage) {
        print_error(code_error_alloc);
        return -1;
    }
    int capacity = 0;
    bool is_trace = false;
    printf("%d %s\n", !strcmp("/trace", argv[2]), argv[2]);
    if (argc == 4 && !strcmp("/trace", argv[2])) {
        is_trace = true;
        st_act = process(argv[1], storage, &capacity, is_trace, argv[3]);
    } else {
        st_act = process(argv[1], storage, &capacity, is_trace, NULL);
    }
    print_error(st_act);
    free_storage(storage, capacity);
    return 0;
}