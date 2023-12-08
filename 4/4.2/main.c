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
    printf("\n");
    printf("[Load] <Name, filename> - to load number to Array from file\n");
    printf("[Save] <Name, filename> - to save numbers from Array to file\n");
    printf("[Rand] <Name, count, lb, rb> - to fill array by random numbers in [lb, rb]\n");
    printf("[Concat] <Name_1, Name_2> - concat two arrays\n");
    printf("[Free] <Name> - to free array\n");
    printf("[Remove] <Name, len, index> - to delete <len> numbers after Name[index]\n");
    printf("[Copy] <Name_1, ind_st, ind_end, Name_2> - to copy elements from Name_1 to Name_2\n");
    printf("[Sort] <Name, A+/A-> - to sort array ascending/descending\n");
    printf("[Shuffle] <Name> - to shuffle array\n");
    printf("[Stats] <Name> - to print info about Name\n");
    printf("[Print] <Name, index/diap/all> - to print elements of Name\n");
    printf("[Exit] - to exit the programm\n");
    printf("\n");
}

status_code get_line(char** arg) {
    int index = 0;
    (*arg) = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!(*arg)) return code_error_alloc;
    char symbol = getchar();
    //printf("%d--", toascii(symbol));
    while (symbol != '\n') {
        (*arg)[index++] = symbol;
        symbol = getchar();
        //printf("%d--", toascii(symbol));
    }
    (*arg)[index] = '\0';
    return code_success;
}

void print_storage(Array** st, int capacity) {
    for (int i = 0; i < capacity; i++) {
        printf("%c\n", st[i]->name);
        print(st[i], st[i]->name, "all");
    }
}

int main(int argc, char* argv[]) {
    status_cmd st_cmd;
    status_code st_act;
    Array** storage = NULL;
    storage = (Array**)malloc(sizeof(Array) * 32);
    int capacity = 0;
    while (true) {
        print_menu();
        char* choose = NULL;
        st_act = get_line(&choose);
        if (st_act == code_error_alloc) {
            printf("Error alloc detected!!!\n");
            break;
        }
        printf("%s0---\n", choose);
        st_cmd = command(choose, storage, &capacity);
        //print_storage(storage, capacity);
        switch (st_cmd) {
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_error_oppening:
                printf("Can`t open file!!!\n");
                break;
            case cmd_exit:
                break;
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            default:
                break;
        }
        free(choose);
        choose = NULL;
        if (st_cmd != cmd_success && st_cmd != cmd_invalid_parameter) {
            break;
        }
    }    
    free_storage(storage, capacity);
    return 0;
}