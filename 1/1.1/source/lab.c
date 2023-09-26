#include "../headers/lab.h"

void print_natural_number(int num) {
    if (num == 0 || num > HIGH_LIMIT_NATURAL_NUM) {
        printf("There are no such numbers\n");
        return;
    }
    for (int i = num; i <= HIGH_LIMIT_NATURAL_NUM; i += num) {
        printf("%d ", i);
    }
}

status_code is_prime_num(int num, bool* result) {
    if (num == 0) {
        *result = true;
        return code_succes;
    }
    if (num < 0) {
        *result = false;
        return code_invalid_parameter;
    }

    int count_digits = 0;
    for (int i = 1; i < sqrt(num) + 1; i++) {
        if (num % i == 0) {
            count_digits++;
            if (num / i == i) {
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

int get_length(int num) {
    if (num == 0) {
        return 1;
    }
    while (num % 10 == 0) {
        num /= 10;
    }
    int count = 0;
    while(num) {
        count++;
        num /= 10;
    }
    return count;
}

status_code partition_number(int num, char** result, int len_number) {
    if (num == 0) {
        *result = (char*)malloc(2);
        if (*result == NULL) {
            return code_overflow; 
        }
        strcpy(*result, "0");
        return code_succes;
    }

    while (num % 10 == 0) {
        num /= 10;
    }
    int res_size = 2 * len_number - 1;
    char res[res_size];
    res[res_size - 1] = '\0';
    for (int i = res_size - 1; i >= 0; i -= 2) { // Начинаем с конца массива и устанавливаем первый символ
        int digit = num % 10;
        res[i] = digit + '0';
        if (i != 0) {
            res[i - 1] = ' ';
        }
        num /= 10;
    }

    *result = (char*)malloc(res_size + 1);
    strcpy(*result, res);

    return code_succes;
}



status_code print_pow(int num_pow, int num) {
    if (num_pow > 10) {
        return code_invalid_parameter;
    }
    int check_res = 1;
    int tmp_res;
    for (int i = 0; i < num; i++) {
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
        res = NULL; 
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

