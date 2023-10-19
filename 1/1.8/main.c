#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"


int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Your programm must start with: %s <FILE_IN> <FILE_OUT>\n", argv[0]);
        exit(1);
    }
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Can`t open file!!!\n");
        exit(2);
    }
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        fclose(input);
        printf("Can`t open file!!!\n");
        exit(2);
    }
    int base;
    char* input_value;
    char* number_no_zeros;
    int decimal_number;
    while(true) {
        bool is_minus = false;
        status_input check_input = scan_num_char(input, output, &input_value, &is_minus);
        if (check_input == status_error_malloc) {
            printf("Error malloc deetected!!!\n");
            exit(1);
        } else if (check_input == status_stop) {
            break;
        }
        status_code find_min = check_min_base(input_value, &base);
        switch (find_min) {
            case code_success:
                fprintf(output, "min base = %d ", base);
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                free(input_value);
                exit(3);
            case code_error_malloc:
                printf("Error malloc detected!!!\n");
                exit(4);
        }
        status_code decimal_status = convert_to_decimal(input_value, base, &decimal_number);
        switch (decimal_status) {
            case code_success:
                if (is_minus) {
                    decimal_number *= -1;
                }
                fprintf(output, "dec = %d ", decimal_number);
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                free(input_value);
                exit(3);
            case code_error_malloc:
                printf("Error malloc detected!!!\n");
                exit(4);
        }
        
        status_code number_no_zeros_status = convert_from_decimal(decimal_number, base, &number_no_zeros);
        switch (number_no_zeros_status) {
            case code_success:
                fprintf(output, "no zeros = %s ", number_no_zeros);
                break;
            case code_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                free(input_value);
                free(number_no_zeros);
                exit(3);
            case code_error_malloc:
                printf("Error malloc detected!!!\n");
                exit(4);
        }
        free(input_value);
        fprintf(output, "\n");
        free(number_no_zeros);
    } 
    fclose(input);
    fclose(output);
 
    return 0;
}