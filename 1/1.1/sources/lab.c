#include "../headers/lab.h"

status_code print_natural_number(int num) {
    if (num < 0) {
        return code_invalid_parameter;
    }
    if (num == 0 || num > HIGH_LIMIT_NATURAL_NUM) {
        return code_invalid_parameter;
    }
    for (int i = num; i <= HIGH_LIMIT_NATURAL_NUM; i += num) {
        printf("%d ", i);
    }
    return code_succes;
}

status_code is_prime_num(int num, bool* result) {
    if (num == 0 || num == 1 || num < 0) {
        *result = false;
        return code_invalid_parameter;
    }
    int count_digits = 2; 
    for (int i = 2; i * i <= num; ++i) { 
        if (num % i == 0) {
            count_digits++;
            if (i != num / i) {
                count_digits++;
            }
        }
    }
    if (count_digits == 2) {
        *result = true;
        return code_succes; 
    } else {
        *result = false;
        return code_succes; 
    }
}

status_code print_pow(int num_pow, int num) {
    if (num_pow > 10) {
        return code_invalid_parameter;
    }
    int check_res = 1;
    int tmp_res;
    for (int i = 0; i <= num; i++) {
        if (i > 0) {
            check_res = tmp_res;
        }
        if (num_pow == 0) {
            tmp_res = 1;
        } else {
            tmp_res = num_pow * check_res;
        }
        if (check_res > tmp_res) {
            return code_overflow;
        }
        printf("| Power of %-5d | %-20d |\n", i, tmp_res);

    }
    return code_succes;
}

status_code ariphm_progression(int num, int* result) {
    if (num == 0) {
        result = NULL;
        return code_invalid_parameter;
    }
    /* S_n = (2 + (n - 1)) * (n / 2)*/
    int right_factor = num / 2;
    int left_factor = 2 + num - 1;
    if (num > HIGH_LIMIT_PROGRESSION) {
        result = NULL;
        return code_overflow;
    }
    *result = left_factor * right_factor;
    return code_succes;

}

status_code factorial(int num, long* res) {
    if (num < 0) {
        //res = NULL; 
        return code_invalid_parameter;
    }
    if (num == 0 || num == 1) {
        *res = 1;
        return code_succes;
    }
    long prev_res = 1;
    *res = 1;
    for (int i = 2; i <= num; i++) {
        *res *= i; 
        if (*res < prev_res) {
            return code_overflow;
        }
        prev_res = *res;
    }
    return code_succes;
}

void partition_number(const char* num) {
    int length = strlen(num);
    if (length == 1) {
        printf("%s\n", num);
        return;
    }
    int last_index = length - 1;
    while (last_index >= 0 && num[last_index] == '0') {
        last_index--;
    }
    for (int i = 0; i <= last_index; i++) {
        printf("%c", num[i]);
        if (i != last_index) {
            printf(" ");
        }
    }

    printf("\n");
}