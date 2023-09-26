#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        printf("Your programm must start with: %s FLAG= NUMBER=\n", argv[0]);
        exit(1);
    }
    int num = atoi(argv[2]);
    char* flag = (char*)malloc(strlen(argv[1]) + 1);
    strcpy(flag, argv[1]);

    if (!strcmp(flag, "/h") || !strcmp(flag, "-h")) {
        print_natural_number(num);
    } else if (!strcmp(flag, "/p") || !strcmp(flag, "-p")) {
        bool result;
        switch(is_prime_num(num, &result)) {
            case code_succes:
                if (result == true) {
                    printf("Your number is prime\n");
                } else {
                    printf("Your number is composite\n");
                }
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            case code_overflow:
                printf("Overflow detected!!!\n");
                break;
        }
    } else if (!strcmp(flag, "/s") || !strcmp(flag, "-s")) {
        int length = get_length(num);
        char* result = NULL;
        switch(partition_number(num, &result, length)) {
            case code_succes:
                printf("String of a number %d: %s\n", num, result);
                free(result);
                break;
            default:
                printf("Something went wrong\n");
                break;
        }
    } else if (!strcmp(flag, "/e") || !strcmp(flag, "-e")) {
        int number_to_pow;
        printf("Enter the number to raise to the power %d\n", num);
        scanf("%d", &number_to_pow);
        switch (print_pow(number_to_pow, num)) {
        case code_succes:
            printf("Everything is ok, no errors detected\n");
            break;
        case code_overflow:
            printf("Overflow detected!!!\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        }
    } else if (!strcmp(flag, "/a") || !strcmp(flag, "-a")) {
        int result;
        switch (ariphm_progression(num, &result)) {
            case code_succes:
                printf("Result: %d\n", result);
                break;
            case code_overflow:
                printf("Overflow detected!!!\n");
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
        }
    } else if (!strcmp(flag, "/f") || !strcmp(flag, "-f")) {
        long result;
        switch (factorial(num, &result)) {
            case code_succes:
                printf("Result: %ld\n", result);
                break;
        
            case code_overflow:
                printf("Overflow detected!!!\n");
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break; 
        }
    } else {
        printf("Unknown flag!\n");
    }
    free(flag);
    return 0;

}