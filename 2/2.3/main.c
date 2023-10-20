#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum status_code {
    code_success,
    code_error_open_file,
    code_invalid_parameter
} status_code;

status_code find_substr(const char* substr, FILE* file, bool* found, int* index_row, int* index_symb) {
    int index = 0;
    char symb = fgetc(file);
    int len = strlen(substr);
    *index_row = 1;
    *index_symb = 0;
    bool catch = false;
    int index_catch = -1;
    while (symb != EOF) {
        printf("%d-%d\n", toascii(symb), toascii(substr[index]));
        if (toascii(symb) == toascii(substr[index]) || (symb == '\r' && (substr[index] == '\n' || substr[index] == '\r'))) {
            printf("yes\n");
            if (!catch && index_catch == -1) {
                catch = true;
                index_catch = *index_symb;
            }
            index++;
            if (index == len) {  
                *found = true;
                break;
            }
        } else {
            index = 0;
            catch = false;
            index_catch = -1;
        }
        
        (*index_symb)++;
        symb = fgetc(file);
        if (symb == '\n') {
            (*index_row)++;
            *index_symb = 0;
        }
    }
    *index_symb = index_catch;

    return code_success;  
}

status_code find_first_in(char* substr, int count, ...) {
    if (count < 1 || substr == NULL) {
        return code_invalid_parameter;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        bool catch = false;
        int index_row, index_symb;
        const char* filename = va_arg(ptr, const char*);
        FILE* file = fopen(filename, "r");

        if (!file) {
            va_end(ptr);
            return code_error_open_file;
        }

        status_code st_find = find_substr(substr, file, &catch, &index_row, &index_symb);

        if (st_find == code_invalid_parameter) {
            va_end(ptr);
            fclose(file);
            return code_invalid_parameter;
        }

        if (catch) {
            printf("substr: %s, found in file: %s, at %d row, at %d symbol\n", substr, filename, index_row, index_symb);
        } else {
            printf("%s not found in %s!\n", substr, filename);
        }

        fclose(file);
    }

    va_end(ptr);
    return code_success;
}

int main(int argc, char* argv[]) {
     /*status_code st_find = find_first_in("zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");*/
    status_code st_find;
    /*
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }
    */
    /*
    st_find = find_first_in("   zxc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");
    printf("\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }
    */
    st_find = find_first_in("z\nx\nc", 4, "input.txt", "input1.txt", "input3.txt", "input4.txt");
    printf("\n");
    if (st_find == code_error_open_file) {
        printf("Can't open file!\n");
        exit(1);
    } else if (st_find == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        exit(2);
    }


    return 0;
}
