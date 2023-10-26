#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    int pow = 0;
    printf("Enter a power to make 2^n base: ");
    if (!scanf("%d", &pow)) {
        printf("Invalid parameter detected!!!\n");
        return -1;
    }
    if (pow < 1 || pow > 5) {
        printf("Invalid parameter detected!!!\n");
        return -2;
    }
    int number = 0;
    int base;
    status_code st_pow = power(2, pow, &base);
    printf("Enter a number to convert: ");
    if (!scanf("%d", &number)) {
        printf("Invalid parameter detected!!!\n");
        return -3;
    }
    char* number_converted = NULL;
    status_code st_convert = convert_from_decimal(number, base, &number_converted);
    if (st_convert == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free(number_converted);
        return -4;
    } else {
        printf("%d in %d base is %s\n", number, base, number_converted);
    }
    free(number_converted);
    return 0;

}