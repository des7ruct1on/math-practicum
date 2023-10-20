#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_ok;
    } else {
        return status_fail;
    }
}

status_code convert_to_decimal(char digit, int * dec_number) {
    if ('0' <= digit && digit <= '9') {
        *dec_number = digit - '0';
    } else if ('A' <= digit && digit <= 'Z') {
        *dec_number = 10 + digit - 'A';
    } else if ('a' <= digit && digit <= 'z') {
        *dec_number = 10 + digit - 'a';
    } else {
        return code_invalid_parameter; 
    }    if ('0' <= digit && digit <= '9') {
        *dec_number = digit - '0';
    } else if ('A' <= digit && digit <= 'Z') {
        *dec_number = 10 + digit - 'A';
    } else if ('a' <= digit && digit <= 'z') {
        *dec_number = 10 + digit - 'a';
    } else {
        return code_invalid_parameter; 
    }
    return code_success;
}

status_code get_char_value(int value, int base, char* res) {
    if (value >= 0 && value < base) {
        if (value < 10) {
            *res = '0' + value;
        } else {
            *res = 'A' + (value - 10);
        }
    } else {
        return code_invalid_parameter;
    }
    return code_success;
}

status_code add_numbers(char** result, int base, int* size, char* number2) {
    if (!(*result) || !number2 || base < 2 || base > 36) {
        return code_invalid_parameter;
    }
    int lenght_number1 = strlen(*result);
    int lenght_number2 = strlen(number2);
    int max_len = fmax(lenght_number1, lenght_number2);
    status_realloc st_realloc;
    if (max_len > strlen(*result)) {
        *size *= 2;
        st_realloc = my_realloc(result, *size);
        //printf("%d---\n", *size);
        if (st_realloc == code_alloc_fail) {
            free(*result);
            return code_alloc_fail;
        }
    }
    int index = max_len;
    int carry = 0;
    int i = 0;
    status_code st_convert;
    while (true) {
        if (lenght_number1 - i - 1 < 0 || lenght_number2 - i - 1 < 0) {
            break;
        }
        int value1, value2;
        char digit_1 = (*result)[lenght_number1 - 1 - i];
        char digit_2 = number2[lenght_number2 - 1 - i];
        //printf("digit1 = %c, digit2 = %c\n", digit_1, digit_2);
        st_convert = convert_to_decimal(digit_1, &value1);
        if (st_convert == code_invalid_parameter) {
            //free_all(1, r_number2);
            return code_invalid_parameter;
        }
        st_convert = convert_to_decimal(digit_2, &value2);
        if (st_convert == code_invalid_parameter) {
            ///free_all(1, r_number2);
            return code_invalid_parameter;
        }
        int tmp_res = value1 + value2 + carry;
        //printf("tmp_ress = %d\n\n", tmp_res);
        carry = tmp_res / base;
        tmp_res %= base;
        //printf("tmp_res = %d, carry = %d\n", tmp_res, carry);
        char new_digit;
        st_convert = get_char_value(tmp_res, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            //free_all(1, r_number2);
            return code_invalid_parameter;
        }
        (*result)[index] = new_digit;
        //printf("\tNEW DIGIT: %c, INDEX = %d\n",new_digit, index);
        index--;
        i++;
    }
    while (lenght_number1 - i > 0) {
        if (*size - 1 == i) {
            //printf("yes\n");
            st_convert = my_realloc(result, *size * 2);
            if (st_convert == code_alloc_fail) {
                //free_all(2, r_number2, result);
                return code_alloc_fail;
            }
            *size *= 2;
        }
        int value1;
        char digit_1 = (*result)[lenght_number1 - 1 - i];
        st_convert = convert_to_decimal(digit_1, &value1);
        if (st_convert == code_invalid_parameter) {
            //free_all(1, r_number2);
            return code_invalid_parameter;
        }
        int tmp_res = value1 + carry;
        carry = tmp_res / base;
        tmp_res %= base;
        char new_digit;
        st_convert = get_char_value(tmp_res, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            //free_all(1, r_number2);
            return code_invalid_parameter;
        }
        //printf("\tNEW DIGIT: %c, INDEX = %d\n",new_digit, index);
        (*result)[index] = new_digit;
        index--;
        i++;
    }
    while (lenght_number2 - i > 0) {
        if (*size - 1 == i) {
            //printf("yes\n");
            st_convert = my_realloc(result, *size * 2);
            if (st_convert == code_alloc_fail) {
                //free_all(2, r_number2, result);
                return code_alloc_fail;
            }
            *size *= 2;
        }
        int value2;
        char digit_2 = number2[lenght_number2 - 1 - i];
        st_convert = convert_to_decimal(digit_2, &value2);
        if (st_convert == code_invalid_parameter) {
            //free_all(1, r_number2);
            return code_invalid_parameter;
        }
        int tmp_res = value2 + carry;
        //printf("tmp_res: %d\n", tmp_res);
        carry = tmp_res / base;
        tmp_res %= base;
        char new_digit;
        st_convert = get_char_value(tmp_res, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            return code_invalid_parameter;
        }
        //printf("\tNEW DIGIT1: %c, INDEX = %d\n",new_digit, index);
        (*result)[index] = new_digit;
        index--;
        i++;
    }
    //printf("%d----\n", carry);
    if (carry) {
        char new_digit;
        st_convert = get_char_value(carry, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            return code_invalid_parameter;
        }
        (*result)[index] = new_digit;
        index--;
    } else {
        for (int k = 0; k < max_len; k++) {
            index++;
            (*result)[k] = (*result)[index];
            //printf("%c>\n", (*result)[k]);
        }
    }
    (*result)[index] = '\0';
    //free_all(1, r_number2);
    return code_success;
}

status_code remove_zeros(char* number, char** number_new) {
    int i = 0; 
    int length = strlen(number);
    while (number[i] == '0' && length != 1) {
        i++;
    }
    //printf("<%d> %d\n", i, length);
    if (i == 0) {
        return code_success;
    } else {
        if (i == length) {
            (*number_new) = (char*)malloc(sizeof(char) * 2);
            if (*number_new == NULL) {
                return code_alloc_fail;
            }
            (*number_new)[0] = '0';
            (*number_new)[1] = '\0';
            return code_success;
        }
        int new_len = length - i;
        (*number_new) = (char*)malloc(sizeof(char) * (length - i + 1));
        if (*number_new == NULL) {
            return code_alloc_fail;
        }
        for (int k = 0; k < new_len; k++) {
            (*number_new)[k] = number[i];
            i++;
        }
        (*number_new)[new_len] = '\0';
    }
    return code_success;
}

status_code sum_numbers_by_base(int base, char** result,int count,...) {
    if (count < 0 || base < 2 || base > 36) {
        return code_invalid_parameter;
    }
    (*result) = (char*)malloc((STR_SIZE + 1) & sizeof(char));
    if (*result == NULL) {
        return code_alloc_fail;
    }
    int size = STR_SIZE + 1;
    va_list ptr;
    char* current_number;
    char* current_number_no_zeros = NULL;
    bool has_zeros = false;
    va_start(ptr, count);
    for (int i = 0; i < count; i++) {
        current_number = va_arg(ptr, char*);
        if (!is_valid_input(current_number, base)) {
            free(*result);
            return code_invalid_parameter;
        }
        status_code st_remove = remove_zeros(current_number, &current_number_no_zeros);\
        if (current_number_no_zeros != NULL) {
            has_zeros = true;
        }
        if (i == 0) {
            if (has_zeros) {
                *result = strdup(current_number_no_zeros);
            } else {
                *result = strdup(current_number);
            }
            if (*result == NULL) {
                return code_invalid_parameter;
            }
        } else {
            status_code status_add;
            if (has_zeros) {
                status_add = add_numbers(result, base, &size, current_number_no_zeros);
            } else {
                status_add = add_numbers(result, base, &size, current_number);
            }

            if (status_add != code_success) {
                //free(result);
                va_end(ptr);
                return code_alloc_fail;
            }
        }
        free(current_number_no_zeros);
        has_zeros = false;

    }
    //printf("%s>>\n", *result);
    va_end(ptr);
    return code_success;

}

bool is_valid_input(char* input_str, int base) {
    for (int i = 0; i < strlen(input_str); ++i) {
        if (i == 0 && input_str[i] == '-') {
            continue;
        }
        if (!(input_str[i] >= 'A' && input_str[i] <= 'Z') && !(input_str[i] >= '0' && input_str[i] <= '9')) {
            return false;
        }
        if (input_str[i] >= '0' && input_str[i] <= '9' && (input_str[i] - '0') >= base) {
            return false;
        }
        if (input_str[i] >= 'A' && input_str[i] <= 'Z' && (input_str[i] - 55) >= base) {
            return false;
        }
    }
    return true;
}


status_realloc free_all(int count, ...) {
    if (count < 1) {
        return status_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_ok;
}