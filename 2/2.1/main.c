#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main (int argc, const char *argv[]) {
    if (argc < 3) {
        printf("Your programm must start at least with: %s <FLAG> <STRING>\n", argv[0]);
        exit(1);
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/' symbol!\n");
        exit(2);
    }
    char flag = argv[1][1];
    int length;
    unsigned int SEED;
    char* reversed_str;
    char* sorted_str;
    char* upper_str;
    char* concat_str;
    switch (flag) {
        case 'l':
            length = my_strlen(argv[2]);
            printf("Length: %d\n", length);
            break;
        case 'r':
            switch (get_reversed_string(argv[2], &reversed_str)) {
                case code_succes:
                    print_s(reversed_str);
                    printf("\n");
                    free(reversed_str);
                    break;
                case code_malloc_error:
                    printf("Error malloc detected!!!\n");
                    break;
            }
            break;
        case 'u':
            switch (get_odd_to_upper_string(argv[2], &upper_str)) {
                case code_succes:
                    print_s(upper_str);
                    printf("\n");
                    free(upper_str);
                    break;
                case code_malloc_error:
                    printf("Error malloc detected!!!\n");
                    break;
            }
            break;
        case 'n':
            switch (get_sorted_string(argv[2], &sorted_str)) {
                case code_succes:
                    print_s(sorted_str);
                    printf("\n");
                    free(sorted_str);
                    break;
                case code_malloc_error:
                    printf("Error malloc detected!!!\n");
                    break;
            }
            break;
        case 'c':
            if (argc < 4) {
                printf("Your programm must start with: %s <FLAG> <STRINGS> <SEED>\n", argv[0]);
                exit(1);
            }
            if (!sscanf(argv[3], "%u", &SEED)) {
                printf("Invalid type of input seed!!!\n");
                exit(3);
            }
            switch (get_concat_strings(argv, argc, &concat_str, SEED)) {
                case code_succes:
                    print_s(concat_str);
                    printf("\n");
                    free(concat_str);
                    break;
                case code_malloc_error:
                    printf("Error malloc detected!!!\n");
                    break;
            }
            break;
        default:
           printf("Unknown flag!\n"); 

    }
    return 0;
}