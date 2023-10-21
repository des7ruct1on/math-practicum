#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_ok;
    } else {
        return status_fail;
    }
}

void print_ins(First_in* ins, int size) {
    for (int i = 0; i < size; i++) {
        First_in tmp = ins[i];
        if (tmp.index_row != 0) {
            printf("row: %d, index: %d in file: %s\n", tmp.index_row, tmp.index_symb, tmp.filename);
        }
    }
}

status_code find_substr(First_in** ins, int* size, int* ind_inv, const char* filename, const char* substr, FILE* file) {
    int index = 0;
    char symb = fgetc(file);
    int len = strlen(substr);
    bool catch = false;
    int index_symb_catch = -1;
    int index_row_catch = -1;
    int index_row = 1; 
    int index_symb = 0;
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
                index_symb_catch = index_symb;
                index_row_catch = index_row;
                //printf("%d>>\n", index_catch);
            }
            index++;
            if (index == len) {  
                First_in tmp;
                tmp.index_row = index_row_catch;
                tmp.index_symb = index_symb_catch;
                tmp.filename = strdup(filename);
                (*ins)[*ind_inv] = tmp;
                (*ind_inv)++;
                //printf("    row: %d, ind: %d\n", tmp.index_row, tmp.index_symb);
                if (*size  == *ind_inv - 1) {
                    *size *= 2;
                    status_realloc st_realloc = my_realloc(ins, *size);
                    if (st_realloc == status_fail) {
                        free(*ins);
                        return code_error_malloc;
                    }
                }
                //printf("substr: %s, found at %d row, at %d symbol\n", substr, index_row_catch, index_symb_catch);
                fseek(file, -len + 1,SEEK_CUR);
                index_symb -= len - 1;
                index_row_catch = -1;
                index_symb_catch = -1;
                index = 0;
                catch = false;
                //break;
            }
        } else {
            index = 0;
            catch = false;
            index_symb_catch = -1;
            index_row_catch = -1;
        }
        
        index_symb++;
        symb = fgetc(file);
        if (ascii_symb == 10) {
            index_row++;
            index_symb = 0;
        }
    }
    //int tmp_sz = *size;
    //print_ins(*ins, tmp_sz);

    return code_success;  
}

status_code find_first_in(First_in** ins, int* size, char* substr, int count, ...) {
    if (count < 1 || substr == NULL) {
        return code_invalid_parameter;
    }
    (*ins) = (First_in*)malloc(sizeof(First_in) * 32);
    if (*ins == NULL) {
        return code_error_malloc;
    }
    *size = 32;
    int index = 0;
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
        status_code st_find = find_substr(ins, size, &index, filename, substr, file);
        if (st_find == code_invalid_parameter) {
            va_end(ptr);
            fclose(file);
            return code_invalid_parameter;
        } else if (st_find == code_error_malloc) {
            va_end(ptr);
            fclose(file);
            return code_error_malloc;
        }

        fclose(file);
    }

    va_end(ptr);
    return code_success;
}
