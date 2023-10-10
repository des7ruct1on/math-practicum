#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

typedef enum status_input {
    status_succes,
    status_stop,
    status_error_malloc
} status_input;

status_input scan_num_char(char** number_str) {

} 

int main(int argc, char* argv[]) {
    int base;
    printf("Enter the base value [2 - 36]: ");
    scanf("%d", &base);
    if (base < 2 || base > 36) {
        printf("Invalid parameter of base!!!\n");
        exit(1);
    }
    char* input_value;
    int decimal_max = 0;
    int len_max_num = 0;
    bool started = false;
    while(true) {
        status_input input = scan_num_char(&input_value);
        if (input == status_error_malloc) {
            printf("Error malloc detected!!!\n");
            exit(1);
        } else if (input == status_stop) {
            break;
        }
        started = true;
        int decimal_tmp;
        status_code check_valid = convert_to_decimal(input_value, base, &decimal_tmp);
        if (check_valid == code_invalid_parameter) {
            printf("Invalid parameter detected!!!\n");
            exit(1);
        } else {
            if (decimal_tmp > decimal_max || decimal_tmp == 0) {
                decimal_max = decimal_tmp;
                len_max_num = strlen(input_value);
            }
        }
    } 
    if (!started) {
        printf("You did not enter a single number\n");
        return 0;
    }
    char* max_value = (char*)malloc((len_max_num + 1) * sizeof(char));
    if (max_value == NULL) {
        printf("Error malloc detected!!!\n");
        exit(1);
    }
    
    status_code from_decimal_status = convert_from_decimal(decimal_max, base, max_value);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        exit(1);
    }
    printf("Max input number in %d base is: %s\n", base, max_value);
    int max_num_no_zeros = remove_trailing_zeros(decimal_max);
    char* max_base_no_zeros = NULL;
    char* max_nine_base = NULL;
    char* max_eighteen_base = NULL;
    char* max_twenty_seven_base = NULL;
    char* max_thirty_six_base = NULL;
    from_decimal_status = convert_from_decimal(max_num_no_zeros, base, &max_base_no_zeros);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        exit(1);
    }
    from_decimal_status = convert_from_decimal(decimal_max, 9, &max_nine_base);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        free(max_base_no_zeros);
        exit(1);
    }
    from_decimal_status = convert_from_decimal(decimal_max, 18, &max_eighteen_base);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        free(max_base_no_zeros);
        free(max_nine_base);
        exit(1);
    }
    from_decimal_status = convert_from_decimal(decimal_max, 27, &max_twenty_seven_base);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        free(max_base_no_zeros);
        free(max_nine_base);
        free(max_eighteen_base);
        exit(1);
    }
    from_decimal_status = convert_from_decimal(decimal_max, 36, &max_thirty_six_base);
    if (from_decimal_status == code_error_malloc) {
        printf("Error malloc detected!!!\n");
        free(max_base_no_zeros);
        free(max_nine_base);
        free(max_eighteen_base);
        free(max_twenty_seven_base);
        exit(1);
    }
    printf("Max number in input base with no trailing zeros: %s\n", max_base_no_zeros);
    printf("Max number in 9 base: %s\n", max_nine_base);
    printf("Max number in 18 base: %s\n", max_eighteen_base);
    printf("Max number in 27 base: %s\n", max_twenty_seven_base);
    printf("Max number in 36 base: %s\n", max_thirty_six_base);
    free(max_base_no_zeros);
    free(max_nine_base);
    free(max_eighteen_base);
    free(max_twenty_seven_base);
    free(max_thirty_six_base);
    return 0;
}