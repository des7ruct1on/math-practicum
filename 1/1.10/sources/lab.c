#include "../headers/lab.h"

status_code convert_to_decimal(char* number_str, int base, int* dec_number) {
    int decimal_number = 0;
    int power = strlen(number_str) - 1;  
    for (int i = 0; i < strlen(number_str); i++) {
        char digit = number_str[i];
        int digit_value;

        if (isdigit(digit)) {
            digit_value = digit - '0';
        } else if (isalpha(digit)) {
            digit = toupper(digit);
            digit_value = digit - 'A' + 10;
        } else {
            return code_invalid_parameter;
        }
        decimal_number += digit_value * pow(base, power);
        power--;
    }
    *dec_number = abs(decimal_number);
    return code_success;
}

status_code convert_from_decimal(int decimal_number, int base,  char** result) {
    int index = 0;
    (*result) = (char*)malloc(MAX_NUM_SIZE * sizeof(char));
    if(*result == NULL) {
        return code_error_malloc;
    }
    char tmp_res[MAX_NUM_SIZE];
    if (decimal_number == 0) {
        (*result)[index] = '0';
        return code_success;
    }
    while (decimal_number) {
        int remainder = decimal_number % base;
        if (remainder < 10) {
            (*result)[index++] = remainder + '0';
        } else {
            (*result)[index++] = remainder - 10 + 'A';
        }
        decimal_number /= base;
    }
    int length = strlen(*result);
    for (int i = 0; i < length / 2; i++) {
        char tmp = (*result)[i];
        (*result)[i] = (*result)[length - 1 - i];
        (*result)[length - 1 - i] = tmp;
    }
    return code_success;
}

int remove_trailing_zeros(int number) {
    int number_no_zeros = number;
    while (number_no_zeros != 0 && number_no_zeros % 10 == 0) {
        number_no_zeros/= 10;
    }
    return number_no_zeros;
}
