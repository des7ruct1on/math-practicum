#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/string.h"


int main(int argc, char* argv[]) {
    My_string* str = String("wow meow");
    print_str(str);
    printf("%d\n", my_strlen(str));
    string_clear(str);
    free(str);
    My_string* str_a = NULL;
    My_string* str_b = String("kek cheburek");
    int res = my_strcpy(&str_a, str_b);
    printf("%d--\n", res);
    print_str(str_a);
    print_str(str_b);
    string_clear(str_a);
    free(str_a);
    string_clear(str_b);
    free(str_b);
    str_b = String("kolokol");
    My_string* str_c = my_strcpy_new(str_b);
    print_str(str_c);
    string_clear(str_c);
    free(str_c);
    string_clear(str_b);
    free(str_b);
    str_b = String("aa");
    str_c = String("bb");
    int c = concat_strings(&str_b, str_c);
    print_str(str_b);
    string_clear(str_b);
    free(str_b);
    string_clear(str_c);
    free(str_c);
    str_b = String("bb");
    str_c = String("bbb");
    printf("%d--\n", my_strcmp(str_b, str_c));
    string_clear(str_b);
    free(str_b);
    string_clear(str_c);
    free(str_c);
    return 0;
}