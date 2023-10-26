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

status_code power(int base, int exponent, int* res) {
    if (exponent < 0) {
        return code_invalid_parameter;
    } else if (exponent == 0) {
        // Любое число в степени 0 равно 1
        *res = 1;
        return code_success;
    }

    *res = 1;

    while (exponent != 0) {
        if (exponent & 1) {
            *res = multiply(*res, base);
        }
        base = multiply(base, base);
        exponent >>= 1;
    }

    return code_success;
}

int plus(int a, int b) {
    int res = a ^ b;
    int remainder = (a & b) << 1;
    if (remainder != 0) {
        return plus(res, remainder);
    }
    return res;
}

int minus(int a, int b) {
    int neg_b = plus(~b, 1);
    int res = a ^ neg_b;
    int remainder = (a & neg_b) << 1;
    if (remainder != 0) {
        return plus(res, remainder);
    }
    return res;
}

int increment(int x) {
    int mask = 1;
    while (x & mask) {
        x ^= mask;
        mask <<= 1;
    }
    x ^= mask;
    return x;
}

int decrement(int x) {
    int mask = 1;
    while ((x & mask) == 0) {
        x ^= mask;
        mask <<= 1;
    }
    x ^= mask;
    return x;
}

int abs_value(int num) {
    int mask = num >> (sizeof(int) * 8 - 1);
    return (plus(num, mask)) ^ mask;
}

char get_char_number(int number) {
    if (number > 9) {
        return plus(number, 55);
    } else {
        return plus(number, 48);
    }
} 

int multiply(int a, int b) {
    int result = 0;
    int mask = 1;

    while (b != 0) {
        if (b & mask) {
            result = plus(result, a);
        }

        a <<= 1;
        b >>= 1;
    }

    return result;
}

status_code convert_from_decimal(long long int decimal_number, int base, char** result) {
    if (base < 2 || base > 36) {
        return code_invalid_parameter;
    }
    int index = 0;
    bool is_minus = false;
    (*result) = (char*)malloc((plus(32, 1)) * sizeof(char));
    if(*result == NULL) {
        return code_error_alloc;
    }
    int size = 0;
    char tmp_res[32];
    if (decimal_number == 0) {
        (*result)[index] = '0';
        (*result)[plus(index, 1)] = '\0';
        return code_success;
    }
    if (decimal_number < 0) {
        is_minus = true;
        (*result)[index] = '-';
        index = increment(index);
        size = increment(size);
    }
    decimal_number = abs(decimal_number);
    while (decimal_number) {
        long long int remainder = decimal_number & (base - 1);
        if (remainder < 10) {
            (*result)[index] = plus(remainder, '0');
        } else {
            (*result)[index] = plus(minus(remainder, 10), 'A');
        }
        index = increment(index);
        size = increment(size);
        if (size > minus(sizeof(*result), 1)) {
            size = multiply(size, 2);
            status_realloc st_realloc = my_realloc(result, size);
            if (st_realloc == status_realloc_fail) {
                return code_error_alloc;
            }
        }
        decimal_number >>= (int)log2(base);
    }
    (*result)[size] = '\0';
    int length = strlen(*result);
    for (int i = 0, j = 0; length - 1 - j > i; i = increment(i), j = increment(j)) {
        if (is_minus) {
            i = increment(i);
            is_minus = false;
        }
        (*result)[i] ^= (*result)[minus(minus(length, 1), j)] ^= (*result)[i] ^= (*result)[minus(minus(length, 1), j)];
    }
    return code_success;
}