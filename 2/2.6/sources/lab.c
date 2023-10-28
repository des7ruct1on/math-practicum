#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

bool is_digit_str(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool is_lower(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if ((str[i] >= 'A') && (str[i] <= 'Z')) {
            return false;
        }
    }
    return true;
} 

bool is_upper(char* str, int size) {
    for (int i = 0; i < size; i++) {
        if ((str[i] >= 'a') && (str[i] <= 'z')) {
            return false;
        }
    }
    return true;
} 

status_code from_roman_to_int(const char* number_str, int* num) {
    int size = strlen(number_str);
    if (!size) {
        return code_invalid_parameter;
    }
    *num = 0;
    int sign;
    if (number_str[0] == '-') {
        sign = -1;
    } else {
        sign = 1;
    }
    for (int i = size - 1; i >= 0; i--) {
        char symbol = number_str[i];
        switch (symbol) {
            case 'I': 
                if (number_str[i + 1] == 'X' || number_str[i + 1] == 'V') {
                    (*num)--;
                } else {
                    (*num)++;
                }
                break;
            case 'V': 
                (*num) += 5;
                break;
            case 'X': 
                if (number_str[i + 1] == 'L' || number_str[i + 1] == 'C') {
                    (*num) -= 10;
                } else {
                    (*num) += 10;
                }
                break;
            case 'L': 
                (*num) += 50;
                break;
            case 'C': 
                if (number_str[i + 1] == 'D' || number_str[i + 1] == 'M') {
                    (*num) -= 100;
                } else {
                    (*num) += 100;
                }
                break;
            case 'M': 
                (*num) += 1000;
                break;
            default:
                break;
        }
    }
    *num *= sign;
    return code_success;
}

status_code from_zeckendorf(const char* number_z, unsigned int* number) {
    int size = strlen(number_z);
    if (!size) {
        return code_invalid_parameter;
    }
    unsigned int num1 = 0;
    unsigned int num2 = 1;
    unsigned int cur = 1;
    *number = 0;
    //printf("%d>>>\n", size);
    for (int i = 0; i < size - 1; i++) {
        //printf("%d---\n", *number);
        char symb = number_z[i];
        if (symb == '1') {
            (*number) += cur;
        } 
        num1 = num2;
        num2 = cur;
        cur = num1 + num2;
    }
    return code_success;
}


status_code convert_to_decimal(char* number_str, int base, int* dec_number, int flag) {
    //printf("%s--\n", number_str);
    if (base < 2 || base > 36) {
        base = 10;
    }
    long long int decimal_number = 0;
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
            if (flag) {
                digit_value = digit - 'A' + 10;
            } else {
                digit_value = digit - 'a' + 10;
            }
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


int overfscanf(FILE * stream, const char * format, ...) {
    if (!stream) {
        return -1;
    }
    int size_format = strlen(format);
    if (!size_format) {
        return -1;
    }
    int argc = 0;
    for (int i = 0; i < size_format; i++) {
        if (format[i] == '%') {
            if (format[i + 1] != '%') {
                argc++;
            }
        }
    }
    va_list ptr;
    va_start(ptr, argc);
    int index = 0;
    char* flag = NULL;
    int ret_value = 0;
    //printf("%d--\n", argc);
    for (int i = 0; i < argc; i++) {
        int size_flag = 1;
        int capacity_flag = 2;
        while(format[index] != '%' && index < size_format) {
            index++;
        }
        //printf("%d -- %d\n", index, size_format);
        flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
        if (!flag) {
            return -1;
        }
        flag[0] = '%';
        index++;
        while ((isdigit(format[index])) || (isalpha(format[index])) && (index < size_format)) {
            char symbol = format[index];
            flag[size_flag] = symbol;
            size_flag++;
            //printf("%c><\n", symbol);
            if (size_flag == capacity_flag - 1) {
                capacity_flag *= 2;
                status_realloc st_realloc = my_realloc(flag, capacity_flag);
                if (st_realloc == status_realloc_fail) {
                    free(flag);
                    printf("Can`t realloc memory!\n");
                    return -1;
                }
            }
            index++;

        }
        flag[size_flag] = '\0';
        if (!strcmp(flag, "%Ro\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            char tmp[STR_SIZE];
            ret_value += fscanf(stream, "%s", tmp);
            status_code st_zeckendorf = from_roman_to_int(tmp, arg_ptr);
        } else if (!strcmp(flag, "%Zr\0")) {
            unsigned int* arg_ptr = va_arg(ptr, unsigned int*);
            char tmp[STR_SIZE];
            ret_value += fscanf(stream, "%s", tmp);
            status_code st_roman = from_zeckendorf(tmp, arg_ptr);
        } else if (!strcmp(flag, "%Cv\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char number_str[STR_SIZE];
            ret_value += fscanf(stream, "%s", number_str);
            int size = strlen(number_str);
            if (is_upper(number_str, size) && !is_digit_str(number_str, size)) {
                printf("Letters are in upper case!!!\n");
                (*arg_ptr) = 0;
            } else {
                status_code st_convert = convert_to_decimal(number_str, base, arg_ptr, 0);
            }
        } else if (!strcmp(flag, "%CV\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char number_str[STR_SIZE];
            ret_value += fscanf(stream, "%s", number_str);
            int size = strlen(number_str);
            if (is_lower(number_str, size) && !is_digit_str(number_str, size)) {
                printf("Letters are in lower case!!!\n");
                (*arg_ptr) = 0;
            } else {
                status_code st_convert = convert_to_decimal(number_str, base, arg_ptr, 1);
            }
        } else {
            void* tmp_arg = va_arg(ptr, void*);
            ret_value += fscanf(stream, flag, tmp_arg);
        }
        free(flag);
        flag = NULL;
    }
    va_end(ptr);
    return ret_value;
}

int oversscanf(char * buf, const char * format, ...) {
    if (!buf) {
        return -1;
    }
    int size_format = strlen(format);
    if (!size_format) {
        return -1;
    }
    int argc = 0;
    for (int i = 0; i < size_format; i++) {
        if (format[i] == '%') {
            if (format[i + 1] != '%') {
                argc++;
            }
        }
    }
    //printf("%d\n", argc);
    va_list ptr;
    va_start(ptr, argc);
    int index = 0;
    int buf_index = 0;
    char* flag = NULL;
    int ret_value = 0;
    //printf("%d--\n", argc);
    for (int i = 0; i < argc; i++) {
        int size_flag = 1;
        int capacity_flag = 2;
        while(format[index] != '%' && index < size_format) {
            index++;
        }
        //printf("%d -- %d\n", index, size_format);
        flag = (char*)malloc(sizeof(char) * (capacity_flag + 2));
        if (!flag) {
            return -1;
        }
        flag[0] = '%';
        index++;
        while ((isdigit(format[index])) || (isalpha(format[index])) && (index < size_format)) {
            char symbol = format[index];
            flag[size_flag] = symbol;
            size_flag++;
            //printf("%c><\n", symbol);
            if (size_flag == capacity_flag - 1) {
                capacity_flag *= 2;
                status_realloc st_realloc = my_realloc(flag, capacity_flag);
                if (st_realloc == status_realloc_fail) {
                    free(flag);
                    printf("Can`t realloc memory!\n");
                    return -1;
                }
            }
            index++;

        }
        flag[size_flag] = '\0';
        //printf("%d %d\n", i, argc);
        //printf("%s--\n", flag);
        if (!strcmp(flag, "%Ro\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            char tmp[STR_SIZE];
            ret_value += sscanf(buf_index + buf, "%s", tmp);
            buf_index += strlen(tmp) + 1;
            status_code st_zeckendorf = from_roman_to_int(tmp, arg_ptr);
        } else if (!strcmp(flag, "%Zr\0")) {
            unsigned int* arg_ptr = va_arg(ptr, unsigned int*);
            char tmp[STR_SIZE];
            ret_value += sscanf(buf_index + buf, "%s", tmp);
            status_code st_roman = from_zeckendorf(tmp, arg_ptr);
            buf_index += strlen(tmp) + 1;
        } else if (!strcmp(flag, "%Cv\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char number_str[STR_SIZE];
            ret_value += sscanf(buf_index + buf, "%s", number_str);
            int size = strlen(number_str);
            buf_index += size + 1;
            if (is_upper(number_str, size) && !is_digit_str(number_str, size)) {
                printf("Letters are in upper case!!!\n");
                (*arg_ptr) = 0;
            } else {
                status_code st_convert = convert_to_decimal(number_str, base, arg_ptr, 0);
            }
            argc--;
        } else if (!strcmp(flag, "%CV\0")) {
            int* arg_ptr = va_arg(ptr, int*);
            argc++;
            int base = va_arg(ptr, int);
            char number_str[STR_SIZE];
            ret_value += sscanf(buf_index + buf, "%s", number_str);
            int size = strlen(number_str);
            buf_index += size + 1;
            if (is_lower(number_str, size) && !is_digit_str(number_str, size)) {
                printf("Letters are in lower case!!!\n");
                (*arg_ptr) = 0;
            } else {
                //printf("check1\n");
                status_code st_convert = convert_to_decimal(number_str, base, arg_ptr, 1);
                //printf("check1\n");
            }
            argc--;
        } else {
            void* tmp_arg = va_arg(ptr, void*);
            ret_value += sscanf(buf_index + buf, flag, tmp_arg);
            buf_index += strlen(tmp_arg) + 1;
        }
        free(flag);
        flag = NULL;
    }
    va_end(ptr);
    return ret_value;
}