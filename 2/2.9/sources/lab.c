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

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}

int my_len_double(double number) {
    const double epsilon = 1e-15;
    int size = 0;
    while (fabs((int)number - number) > epsilon) {
        number *= 10;
        size++;
    }
    return size;
}

int gcd(int a, int b) {
    while (b) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

void to_common_fraction(double number, int* numerator, int* denumerator, int base) {
    const double epsilon = 1e-15;
    if (fabs(number) < epsilon) {
        *denumerator = base;
        *numerator = 0;
        return;
    }
    int size = my_len_double(number);
    int pow = 1;
    *numerator = 0;
    *denumerator = 1;
    for (int i = 0; i < size; i++) {
        pow *= 10;
        *numerator = *numerator * 10 + ((int)(number * pow) % 10);
        *denumerator *= 10;
    }
    int gcd_value = gcd(*numerator, *denumerator);
    *numerator /= gcd_value;
    *denumerator /= gcd_value;
}

status_code fill_by_primes(int** prime_nums, int* size, int number) {
    if (number < 2) {
        return code_invalid_parameter;
    }
    int index = 0;
    if ((number & 1) == 0) {
        (*prime_nums)[index] = 2;
        index++;
    }
    bool is_prime;
    for (int i = 3; i <= number; i += 2) {
        is_prime = true;
        for (int j = 0; j < index; j++) {
            if (i % (*prime_nums)[j] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime && number % i == 0) {
            (*prime_nums)[index] = i;
            index++;
            if (*size - 1 == index) {
                *size *= 2;
                status_realloc st_realloc = my_realloc(prime_nums, *size);
                if (st_realloc == status_realloc_fail) {
                    return code_error_alloc;
                }
            }
        }
    }
    *size = index;
    return code_success;
}

status_code is_finite_representation(int base, int denumerator, bool* result) {
    int size_number = 32;
    int* prime_nums_number = malloc(sizeof(int) * size_number);
    if (!prime_nums_number) {
        free(prime_nums_number);
        return code_error_alloc;
    }
    status_code st_number_prime = fill_by_primes(&prime_nums_number, &size_number, denumerator);
    if (st_number_prime == code_invalid_parameter) {
        free(prime_nums_number);
    } else if (st_number_prime == code_error_alloc) {
        free(prime_nums_number);
        return code_error_alloc;
    }
    for (int i = 0; i < size_number; i++) {
        if (base % prime_nums_number[i] != 0) {
            *result = false;
            free(prime_nums_number);
            return code_success;
        }
    }
    free(prime_nums_number);
    *result = true;
    return code_success;
}

status_code check_finite(double** res, int base, int* size, int count,...) {
    if (count < 1) {
        return code_invalid_parameter;
    }
    (*res) = (double*)malloc(sizeof(double) * count);
    if (*res == NULL) {
        return code_error_alloc;
    }
    int index = 0;
    va_list ptr;
    va_start(ptr, count);
    for (int i = 0; i < count; i++) {
        int numeratur, denumerator;
        double number = va_arg(ptr, double);
        to_common_fraction(number, &numeratur, &denumerator, base);
        bool has_finite_representation;
        switch(is_finite_representation(base, denumerator, &has_finite_representation)) {
            case code_invalid_parameter:
                va_end(ptr);
                return code_invalid_parameter;
            case code_error_alloc:
                va_end(ptr);
                return code_error_alloc;
            default:
                break;
        }
        if (has_finite_representation) {
            (*res)[index] = number;
            //printf("%f--\n", (*res)[index]);
            index++;
        }

    }
    *size = index;
    va_end(ptr);
    return code_success;
}

void print_arr(double arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%f ", arr[i]);
    }
    printf("\n");
}
