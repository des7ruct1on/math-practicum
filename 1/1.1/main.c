#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        printf("Your programm must start with: %s <FLAG> <NUMBER>\n", argv[0]);
        exit(1);
    }
    int num = atoi(argv[2]);
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/' symbol!\n");
        exit(1);
    }
    char flag = argv[1][1];
    bool result;
    int length;
    char* result_str = NULL;
    int number_to_pow;
    int result_progression;
    long result_fact;
    switch (flag) {
        case 'h':
            print_natural_number(num);
            break;
        case 'p':
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
            break;
        case 's':
            length = get_length(num);
            switch(partition_number(num, &result_str, length)) {
                case code_succes:
                    printf("String of a number %d: %s\n", num, result_str);
                    free(result_str);
                    break;
                default:
                    printf("Something went wrong\n");
                    break;
            }
            break;
        case 'e':
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
            break;
        case 'a':
            switch (ariphm_progression(num, &result_progression)) {
                case code_succes:
                    printf("Result: %d\n", result_progression);
                    break;
                case code_overflow:
                    printf("Overflow detected!!!\n");
                    break;
                case code_invalid_parameter:
                    printf("Invalid parameter detected!!!\n");
                    break;
            }
            break;
        case 'f':
            switch (factorial(num, &result_fact)) {
                case code_succes:
                    printf("Result: %ld\n", result_fact);
                    break;       
                case code_overflow:
                    printf("Overflow detected!!!\n");
                    break;
                case code_invalid_parameter:
                    printf("Invalid parameter detected!!!\n");
                    break; 
            }
            break;
        default:
           printf("Unknown flag!\n"); 

    }
    return 0;
}