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

void reverse_string(char* str) {
    int length = strlen(str);
    int index = 0;
    while(str[index] == '0') {
        index++;
    }
    printf("%d--\n", index);
    for (int i = 0, j = 0; length - 1 - j > index; i++, j++, index++) {
        char tmp = str[index];
        str[index] = str[length - 1 - j];
        str[length - 1 - j] = tmp;
    }
    printf("str: %s\n", str);
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
        st_realloc = my_realloc(result, *size * 2);
    }
    char* r_number2 = malloc(sizeof(char) * (lenght_number2 + 1));
    if (!r_number2) {
        return code_alloc_fail;
    }
    strcpy(r_number2, number2);
    reverse_string(r_number2);
    //printf("%s---\n", r_number2);
    int index = 0;
    int carry = 0;
    status_code st_convert;
    while (true) {
        if (lenght_number1 - index == 0 || lenght_number2 - index == 0) {
            break;
        }
        int value1, value2;
        char digit_1 = (*result)[index];
        char digit_2 = r_number2[index];
        //printf("digit1 = %c, digit2 = %c\n", digit_1, digit_2);
        st_convert = convert_to_decimal(digit_1, &value1);
        if (st_convert == code_invalid_parameter) {
            free_all(1, r_number2);
            return code_invalid_parameter;
        }
        st_convert = convert_to_decimal(digit_2, &value2);
        if (st_convert == code_invalid_parameter) {
            free_all(1, r_number2);
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
            free_all(1, r_number2);
            return code_invalid_parameter;
        }
        (*result)[index] = new_digit;
        //printf("\tNEW DIGIT: %c, INDEX = %d\n",new_digit, index);
        index++;
    }
    while (lenght_number1 - index > 0) {
        if (*size - 1 == index) {
            //printf("yes\n");
            st_convert = my_realloc(result, *size * 2);
            if (st_convert == code_alloc_fail) {
                free_all(2, r_number2, result);
                return code_alloc_fail;
            }
            *size *= 2;
        }
        int value1;
        char digit_1 = (*result)[index];
        st_convert = convert_to_decimal(digit_1, &value1);
        if (st_convert == code_invalid_parameter) {
            free_all(1, r_number2);
            return code_invalid_parameter;
        }
        int tmp_res = value1 + carry;
        carry = tmp_res / base;
        tmp_res %= base;
        char new_digit;
        st_convert = get_char_value(tmp_res, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            free_all(1, r_number2);
            return code_invalid_parameter;
        }
        //printf("\tNEW DIGIT: %c, INDEX = %d\n",new_digit, index);
        (*result)[index] = new_digit;
        index++;
    }
    while (lenght_number2 - index > 0) {
        if (*size - 1 == index) {
            //printf("yes\n");
            st_convert = my_realloc(result, *size * 2);
            if (st_convert == code_alloc_fail) {
                free_all(2, r_number2, result);
                return code_alloc_fail;
            }
            *size *= 2;
        }
        int value2;
        char digit_2 = r_number2[index];
        st_convert = convert_to_decimal(digit_2, &value2);
        if (st_convert == code_invalid_parameter) {
            free_all(1, r_number2);
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
        index++;
    }
    if (carry) {
        char new_digit;
        st_convert = get_char_value(carry, base, &new_digit);
        if (st_convert == code_invalid_parameter) {
            return code_invalid_parameter;
        }
        (*result)[index] = new_digit;
        index++;
    }
    (*result)[index] = '\0';
    free_all(1, r_number2);
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
    va_start(ptr, count);
    for (int i = 0; i < count; i++) {
        current_number = va_arg(ptr, char*);
        if (!is_valid_input(current_number, base)) {
            free(*result);
            return code_invalid_parameter;
        }
        if (i == 0) {
            int size = strlen(current_number);
            char* r_number = malloc((sizeof(char) + 1) * size);
            strcpy(r_number, current_number);
            reverse_string(r_number);
            //printf("3423\n");
            *result = strdup(r_number);
            free_all(1, r_number);
        } else {
            status_code status = add_numbers(result, base, &size, current_number);

            if (status != code_success) {
                //free(result);
                va_end(ptr);
                return code_alloc_fail;
            }
        }

    }
    //printf("%s>>\n", *result);
    reverse_string(*result);
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