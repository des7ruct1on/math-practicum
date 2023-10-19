#include "../headers/lab.h"

status_input scan_num_char(FILE* in, FILE* out, char** number_str, bool* is_minus) {
    (*number_str) = (char*)malloc((MAX_NUM_SIZE + 1) * sizeof(char));
    if (*number_str == NULL) {
        return status_error_malloc;
    }

    int size = 0;
    char symbol = fgetc(in);
    while (isspace(symbol)) {
        symbol = fgetc(in);
    }
    bool start = true;
    while (!isspace(symbol) && symbol != EOF) {
        if (symbol == '-') {
            if (start) {
                (*number_str)[size] = symbol;
                size++;
                symbol = fgetc(in);
                start = false;
                *is_minus = true;
                continue;
            } else {
                (*number_str)[size] = symbol;
                size++;
                symbol = fgetc(in);
                continue;
            }
        }
        start = false;
        symbol = toupper(symbol);
        (*number_str)[size] = symbol;
        size++;
        if (size >= sizeof(*number_str) - 1) {
            (*number_str) = realloc(*number_str, sizeof(char) * size * 2);
            if (*number_str == NULL) {
                free(*number_str);
                return status_error_malloc;
            }
        }
        symbol = fgetc(in);
    }
    (*number_str)[size] = '\0';
    if (size == 0 || (size == 1 && (*number_str)[0] == '-')) {
        free(*number_str);
        return status_stop;
    }
    return status_success;
}


status_code convert_to_decimal(char* number_str, int base, int * dec_number) {
    unsigned int decimal_number = 0;
    int power = strlen(number_str) - 1;  
    bool start = true;
    for (int i = 0; i < strlen(number_str); i++) {
        char digit = number_str[i];
        if (digit == '-' && start) {
            if (start) {
                power--;
                start = false;
                continue;
            }
        }
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
        start = false;
    }
    if (decimal_number > INT_MAX) {
        return code_invalid_parameter;
    }
    *dec_number = decimal_number;
    return code_success;
}

status_code convert_from_decimal(int decimal_number, int base,  char** result) {
    int index = 0;
    bool is_minus = false;
    (*result) = (char*)malloc((MAX_NUM_SIZE + 1) * sizeof(char));
    if(*result == NULL) {
        return code_error_malloc;
    }
    int size = 0;
    char tmp_res[MAX_NUM_SIZE];
    if (decimal_number == 0) {
        (*result)[index] = '0';
        (*result)[index + 1] = '\0';
        return code_success;
    }
    if (decimal_number < 0) {
        is_minus = true;
        (*result)[index++] = '-';
        size++;
    }
    decimal_number = abs(decimal_number);
    while (decimal_number) {
        int remainder = decimal_number % base;
        if (remainder < 10) {
            (*result)[index++] = remainder + '0';
        } else {
            (*result)[index++] = remainder - 10 + 'A';
        }
        size++;
        if (size > sizeof(*result) - 1) {
            (*result) = realloc(*result, size * 2 * sizeof(char));
        }
        decimal_number /= base;
    }
    (*result)[size] = '\0';
    int length = strlen(*result);
    for (int i = 0, j = 0; length - 1 - j > i; i++, j++) {
        if (is_minus) {
            i++;
            is_minus = false;
        }
        char tmp = (*result)[i];
        (*result)[i] = (*result)[length - 1 - j];
        (*result)[length - 1 - j] = tmp;
    }
    return code_success;
}

status_code check_min_base(char* str_number, int* min_base) {
    int size = strlen(str_number);
    
    char tmp_base = '1';
    if (size == 0) {
        return code_invalid_parameter;
    }
    bool start = true;
    for (int i = 0; i < size; i++) {
        char symbol = str_number[i];
        if (symbol == '-' && start) {
            i++;
            start = false;
            continue;
        }
        start = false;
        if (isalpha(symbol) || isdigit(symbol)) {
            if (symbol > tmp_base) {
                tmp_base = symbol;
            }
        } else {
            return code_invalid_parameter;
        }
    }
    if (isalpha(tmp_base)) {
        *min_base = tmp_base - 55 + 1;
    } else {
        *min_base = tmp_base - '0' + 1;
    }
    return code_success;
}