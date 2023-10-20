#include "../headers/lab.h"

status_code find_substr(const char* substr, FILE* file, bool* found, int* index_row, int* index_symb) {
    int index = 0;
    char symb = fgetc(file);
    int len = strlen(substr);
    *index_row = 1;
    *index_symb = 0;
    bool catch = false;
    int index_symb_catch = -1;
    int index_row_catch = -1;

    while (symb != EOF) {
        //printf("%d index\n", *index_symb);
        //printf("%d-%d\n", toascii(symb), toascii(substr[index]));
        if (substr[index] == '\t' && symb == ' ') {
            int count = 1;
            for (count= 1; count < 3; count++) {
                symb = fgetc(file);
                //(*index_symb)++;
                if (symb != ' ') {
                    break;
                }
            }
            if (count == 3) {
                symb = '\t';
            }
            //printf("%c----%c ----%d\n", symb, substr[index], count);
        }
        int ascii_symb = toascii(symb);
        int ascii_substr = toascii(substr[index]);
        if (toascii(symb) == toascii(substr[index])) {
            //printf("yes\n");
            if (!catch && index_symb_catch == -1) {
                catch = true;
                index_symb_catch = *index_symb;
                index_row_catch = *index_row;
                //printf("%d>>\n", index_catch);
            }
            index++;
            if (index == len) {  
                *found = true;
                break;
            }
        } else {
            index = 0;
            catch = false;
            index_symb_catch = -1;
            index_row_catch = -1;
        }
        
        (*index_symb)++;
        symb = fgetc(file);
        if (ascii_symb == 10) {
            (*index_row)++;
            *index_symb = 0;
        }
    }
    *index_symb = index_symb_catch;
    *index_row = index_row_catch;

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
            printf("not found in %s!\n", filename);
        }

        fclose(file);
    }

    va_end(ptr);
    return code_success;
}
