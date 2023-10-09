#include "../headers/lab.h"

int my_strlen(const char* str) {
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        length++;
    }
    return length;
}

void print_s(const char* str) {
    if (str[0] == '\0') {
        return;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
    }
}

int get_my_rand(int min, int max) {
    return rand() % (max - min  + 1) + min;
}

bool my_is_alpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool my_is_digit(char c) {
    return (c >= '0' && c <= '9');
}

bool my_is_upper(char c) {
    return (c >= 'A' && c <= 'Z');
}

char my_to_upper(char c) {
    return c - 'a' + 'A';
}

status_code get_reversed_string(const char* str_old, char** reversed) {
    int length = my_strlen(str_old);
    if (length == 0) {
        return code_succes;
    }
    (*reversed) = (char*)malloc((length + 1) * sizeof(char));
    if (*reversed == NULL) {
        return code_malloc_error;
    }
    for (int i = 0, j = length - 1; j >= 0; i++, j--) {
        (*reversed)[i] = str_old[j];
    }
    (*reversed)[length] = '\0';
    return code_succes;
}

status_code get_odd_to_upper_string(const char* str_old, char** upper) {
    int length = my_strlen(str_old);
    if (length == 0) {
        return code_succes;
    }
    (*upper) = (char*)malloc((length + 1) * sizeof(char));
    if (*upper == NULL) {
        return code_malloc_error;
    }
    for (int i = 0; i < length; i++) {
        char symb = str_old[i];
        if ((i & 1) == 0) {
            if (!my_is_digit(symb) || !my_is_upper(symb)) {
                (*upper)[i] = my_to_upper(symb);
            } else {
                (*upper)[i] = symb;
            }
        } else {
            (*upper)[i] = symb;
        }
    }
    (*upper)[length] = '\0';
    return code_succes;
}

status_code get_sorted_string(const char* str_old, char** sorted) {
    int length = my_strlen(str_old);
    int count_digits = 0;
    int count_letters = 0;
    int count_other_symb = 0;
    for (int i = 0; i < length; i++) {
        if (my_is_alpha(str_old[i])) {
            count_letters++;
        } else if (my_is_digit(str_old[i])) {
            count_digits++;
        } else {
            count_other_symb++;
        }
    }
    (*sorted) = (char*)malloc((length + 1) * sizeof(char));
    if (*sorted == NULL) {
        return code_malloc_error;
    }
    int digit_index = 0;
    int letter_index = 0;
    int other_symb_index = 0;
    for (int i = 0; i < length; i++) {
        if (my_is_alpha(str_old[i])) {
            (*sorted)[count_digits + letter_index] = str_old[i];
            letter_index++;
        } else if (my_is_digit(str_old[i])) {
            (*sorted)[digit_index] = str_old[i];
            digit_index++;
        } else {
            (*sorted)[count_digits + count_letters + other_symb_index] = str_old[i];
            other_symb_index++;
        }
    }
    (*sorted)[length] = '\0';
    return code_succes;
}

void fill_array(int arr[], int size, int a, int b, unsigned int random_seeed) {
    for (int i = 0; i < size; i++) {
        arr[i] = 0;
    }
    srand(random_seeed);
    bool unique_num = true;
    for (int i = 0; i < size;) {
        unique_num = true;
        if (i == 0) {
            arr[i] = get_my_rand(a, b);
            i++;
            continue;
        }
        int value = get_my_rand(a, b);
        for (int j = 0; j < i; j++) {
            if (arr[j] == value) {
                unique_num = false;
                break;  
            }
        }
        if (!unique_num) {
            continue;
        }
        arr[i] = value;
        if (arr[i] == b) {
            arr[i] = 2;
        }
        i++; 
    }
}

status_code get_concat_strings(const char* strings[], int count_argc, char** res_str, unsigned int random_seeed) {
    int res_len = my_strlen(strings[2]);
    for (int i = 4; i < count_argc; i++) {
        res_len += my_strlen(strings[i]);
    }
    (*res_str) = (char*)malloc((res_len + 1) * sizeof(char));
    if (*res_str == NULL) {
        return code_malloc_error;
    }
    int count_strings = count_argc - 3;
    int nums[count_strings];
    int index_res = 0;
    fill_array(nums, count_strings, 4, count_argc, random_seeed);
    /*for (int i = 0; i < count_strings; i++) {
        printf("%d--", nums[i]);
    }
    */
    for (int i = 0; i < count_strings; i++) {
        for (int j = 0; strings[nums[i]][j] != '\0'; j++) {
            (*res_str)[index_res] = strings[nums[i]][j];
            index_res++;
        }
    }
    (*res_str)[res_len] = '\0';
    return code_succes;
}
