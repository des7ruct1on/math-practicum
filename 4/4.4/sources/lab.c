#include "../headers/lab.h"

status_code create_vector(Vector** ar, char c) {
    (*ar) = (Vector*)malloc(sizeof(Vector));
    if (!(*ar)) return code_error_alloc;
    (*ar)->name = toupper(c);
    (*ar)->size = 0;
    (*ar)->capacity = 256;
    (*ar)->base = 10;
    (*ar)->data = (char*)malloc(sizeof(char) * (*ar)->capacity);
    if (!(*ar)->data) {
        free(*ar);
        return code_error_alloc;
    }
    //printf("zdesssssaaaqqq\n");
    return code_success;
}

void print_info_file(FILE* file, condition status, Vector* var) {
    switch (status) {
        case cond_error_reading:
            fprintf(file, "Error reading file\n");
            break;
        case cond_error_writing:
            fprintf(file, "Error writing file\n");
            break;
        case cond_met_comment:
            fprintf(file, "Met comment\n");
            break;
        case cond_unknown_var:
            fprintf(file, "Unknown var!\n");
            break;
        case cond_value:
            fprintf(file, "Var: %c\n", var->name);
            for (int i = 0; i < var->size; i++) {
                fprintf(file, "%c ", var->data[i]);
            }
            fprintf(file, "\n");
            break;
        case cond_invalid_parameter:
            fprintf(file, "Invalid parameter detected!!!\n");
            break;
        default:
            break;
    }
}


status_code add_to_vector(Vector** arr, char c, char number) {
    status_code st_act;
    if (!(*arr)) {
        st_act = create_vector(arr, c);
        if (st_act != code_success) {
            return st_act;
        }
    }
    (*arr)->data[(*arr)->size] = number;
    (*arr)->size++;
    if ((*arr)->size == (*arr)->capacity - 1) {
        char* tmp = realloc((*arr)->data, sizeof(char) * (*arr)->capacity * 2);
        if (!tmp) {
            return code_error_alloc;
        } else {
            (*arr)->data = tmp;
            (*arr)->capacity *= 2;
        }
    }
    return code_success;
}

void free_vector(Vector* arr) {
    if (!arr) return;
    free(arr->data);
    free(arr);
}


Vector* find_vector(Vector** storage, int size, char name) {
    if (!storage) return NULL;
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        char cur = storage[middle]->name; 
        if (cur < name) {
            left = middle + 1;
        } else if (cur > name) {
            right = middle - 1;
        } else {
            return storage[middle];
        }
    }
    return NULL;
}




void free_storage(Vector** st, int size) {
    if (!st) return;

    for (int i = 0; i < size; i++) {
        free_vector(st[i]);
    }
    free(st);
}

void add_to_storage(Vector** st, Vector* a, int* capacity) {
    if (!a) return;
    if (*capacity == 0) {
        st[*capacity] = a;
        (*capacity)++;
    } else {
        int index = 0;
        while (index < *capacity && a->name > st[index]->name) {
            index++;
        }
        for (int i = *capacity; i > index; i--) {
            st[i] = st[i - 1];
        }
        st[index] = a;
        (*capacity)++;
    }
}

status_code operation(Vector** a, Vector* b, char operator[]) {
    //printf("delau %c %s %c\n", (*a)->name, operator, b->name);
    //printf("%s %d\n", operator, strcmp(operator, ":="));
    if (!b) {
        //printf("ZDEfsdfsdS\n");
        return code_invalid_parameter;
    }
    if ((*a)->size != b->size && strcmp(operator, ":=") && strcmp(operator, ":=\\")) {
        //printf("ZDEfsdfsdS\n");
        return code_invalid_parameter;
    }
    if (!strcmp(operator, ":=")) {

        if (b->size > (*a)->size) {
            char* tmp = realloc((*a)->data, sizeof(char) * b->size * 2);
            if (!tmp) {
                return code_error_alloc;
            } else {
                (*a)->data = tmp;
                (*a)->capacity = b->size * 2;
            }
        }
        (*a)->size = b->size;
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = b->data[i];
        }
        //printf("ZDES\n");
    } 
    else if (!strcmp(operator, "+")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = ((*a)->data[i] - '0') | (b->data[i] - '0') + '0';
        }
    }
    else if (!strcmp(operator, "&")) {
        //printf("zashel\n");
        //printf("%d    %d  \n", (*a)->size, b->size);
        for (int i = 0; i < b->size; i++) {
            //printf("%d   %d-\n", (*a)->data[i] - '0', b->data[i] - '0');
            (*a)->data[i] = (((*a)->data[i] - '0') & (b->data[i] - '0')) + '0';
        }
    }
    else if (!strcmp(operator, "->")) {
        //printf("zashel\n");
        //printf("%d---\n", b->size);
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = (((*a)->data[i] - '0') <= (b->data[i] - '0')) + '0';
            //printf("%d-----\n", (*a)->data[i]);
        }
        //printf("str %s\n", (*a)->data);
    }
    else if (!strcmp(operator, "<-")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = (((*a)->data[i] - '0') >= (b->data[i] - '0')) + '0';
        }
    }
    else if (!strcmp(operator, "~")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = (((*a)->data[i] - '0') == (b->data[i] - '0')) + '0';
        }
    }
    else if (!strcmp(operator, "<>")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = ((*a)->data[i] - '0') ^ (b->data[i] - '0') + '0';
        }
    }
    else if (!strcmp(operator, "+>")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = !(((*a)->data[i] - '0') <= (b->data[i] - '0')) + '0';
        }
    }
    else if (!strcmp(operator, "?")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = !(((*a)->data[i] - '0') & (b->data[i] - '0')) + '0';
        }
    }
    else if (!strcmp(operator, "!")) {
        for (int i = 0; i < b->size; i++) {
            (*a)->data[i] = (!((*a)->data[i] - '0') & !(b->data[i] - '0')) + '0';
        }
    } else if (!strcmp(operator, ":=\\")){
        //printf("%d %d\n", (*a)->size, b->size);
        char* tmp = realloc((*a)->data, sizeof(char) * b->size * 2);
        if (!tmp) {
            return code_error_alloc;
        } else {
            (*a)->data = tmp;
            (*a)->capacity = b->size * 2;
        }
        (*a)->size = b->size;
        int i = 0;
        while (b->data[i] - '0' == 0) {
            i++;
        }
        (*a)->size = b->size - i;
        for (int j = 0; i < b->size && j < b->size; j++, i++) {
            //printf("%d %d\n", (b->data[i] - '0'), !(b->data[i] - '0'));
            (*a)->data[j] = !(b->data[i] - '0') + '0';
            //printf("ZAPISALO %c\n", (*a)->data[i]);
        }
    }
    return code_success;
}

bool is_operator(char c) {
    return (c == '\\' || c == '+' || c == '&' || c == '|' || c == '~' || c == '-' || c == '<' || c == '>' || c == '?' || c == '!' || c == ':' || c == '=');
}


status_code convert_from_decimal(int decimal_number, int base,  char** result) {
    int index = 0;
    bool is_minus = false;
    (*result) = (char*)malloc((32 + 1) * sizeof(char));
    if(*result == NULL) {
        return code_error_alloc;
    }
    int size = 0;
    char tmp_res[32];
    if (decimal_number == 0) {
        (*result)[index] = '0';
        (*result)[index + 1] = '\0';
        return code_success;
    }
    if (decimal_number < 0) {
        is_minus = true;
        (*result)[index++] = '-';
        size++;
    }
    decimal_number = abs(decimal_number);
    while (decimal_number) {
        int remainder = decimal_number % base;
        if (remainder < 10) {
            (*result)[index++] = remainder + '0';
        } else {
            (*result)[index++] = remainder - 10 + 'A';
        }
        size++;
        if (size > sizeof(*result) - 1) {
            (*result) = realloc(*result, size * 2 * sizeof(char));
        }
        decimal_number /= base;
    }
    (*result)[size] = '\0';
    int length = strlen(*result);
    for (int i = 0, j = 0; length - 1 - j > i; i++, j++) {
        if (is_minus) {
            i++;
            is_minus = false;
        }
        char tmp = (*result)[i];
        (*result)[i] = (*result)[length - 1 - j];
        (*result)[length - 1 - j] = tmp;
    }
    return code_success;
}

status_code convert_to_decimal(char* number_str, int base, int * dec_number) {
    unsigned int decimal_number = 0;
    int power = strlen(number_str) - 1;  
    bool start = true;
    //printf("here1 %s\n", number_str);
    int size = strlen(number_str);
    //printf("SIZE %d\n", size);
    for (int i = 0; i < size; i++) {
        char digit = number_str[i];
        //printf("%c\n", digit);
        //printf("hzzz\n");
        if (digit == '-' && start) {
            if (start) {
                power--;
                start = false;
                continue;
            }
        }
        int digit_value;
        if (isdigit(digit)) {
            digit_value = digit - '0';
        } else if (isalpha(digit)) {
            digit = toupper(digit);
            digit_value = digit - 'A' + 10;
        } else {
            return code_invalid_parameter;
        }
        decimal_number += digit_value * pow(base, power);
        power--;
        start = false;
    }
    //printf("here2\n");
    if (decimal_number > INT_MAX) {
        return code_invalid_parameter;
    }
    //printf("here3\n");
    *dec_number = decimal_number;
    return code_success;
}


status_code read_vector(Vector** a, int base) {
    if (!*a) return code_invalid_parameter;
    if (base < 2 || base > 36) {
        return code_invalid_parameter;
    }
    printf("enter vector value in %d base\n", base);
    char* number = (char*)malloc(sizeof(char) * 256);
    if (!number) {
        return code_error_alloc;
    }
    int index = 0;
    char c = getchar();
    while (!isspace(c) && c != '\n' && c != '\0') {
        number[index] = c;
        c = getchar();
        index++;
        //printf("%d>>\n", toascii(c));
        if (toascii(c) == 10) {
            break;
        }
    }  
    number[index] = '\0';
    //printf("%s---\n", number);
    int dec_number;
    status_code st_act = convert_to_decimal(number, base, &dec_number);
    if (st_act != code_success) {
        free(number);
        return st_act;
    }
    free(number);
    number = NULL;
    char* new_num = NULL;
    st_act = convert_from_decimal(dec_number, 2, &new_num);
    if (st_act != code_success) {
        free(new_num);
        return st_act;
    }
    (*a)->base = 2;
    int new_size = strlen(new_num);
    (*a)->size = new_size;
    char* tmp = realloc((*a)->data, sizeof(char) * new_size * 2);
    if (!tmp) {
        free(new_num);
        return code_error_alloc;
    } else {
        (*a)->data = tmp;
        (*a)->capacity = new_size * 2;
    }
    for (int i = 0; i < new_size; i++) {
        (*a)->data[i] = new_num[i];
        //printf("%c", (*a)->data[i]);
    }
    free(new_num);
    printf("\n");
    return code_success;
}

status_code print_vector(Vector* a, int base, const char* filename) {
    if (!a) return code_invalid_parameter;
    if (base < 2 || base > 36) {
        return code_invalid_parameter;
    }
    int dec_num;
    status_code st_act;
    //printf("0000\n");
    st_act = convert_to_decimal(a->data, 2, &dec_num);
    if (st_act != code_success) {
        return st_act;
    }
    //printf("1111\n");
    char* new_num = NULL;
    st_act = convert_from_decimal(dec_num, base, &new_num);
    if (st_act != code_success) {
        return st_act;
    }
    //printf("2222\n");
    if (!filename) {
        printf("Number in %d base: %s\n", base, new_num);
    } else {
        FILE* file = fopen(filename, "a+");
        if (!file) {
            free(new_num);
            return code_error_oppening;
        }
        fprintf(file, "Number in %d base: %s\n", base, new_num);
    }
    free(new_num);
    new_num = NULL;
    return code_success;

}

status_code process(const char* filename, Vector** storage, int* size_storage, bool is_trace, const char* output) {
    //printf("%s %s\n", filename, output);
    FILE* file = fopen(filename, "r");
    if (!file) return code_error_oppening;

    char* line = NULL;
    int read;
    bool is_comment = false;
    FILE* file_out;
    if (output) {
        file_out = fopen(output, "a+");
        if (!file_out) {
            fclose(file);
            return code_error_oppening;
        }
    } else {
        file_out = stdout;
    }

    while ((read = getline(&line, &(size_t){0}, file)) != -1) {
        if (read == -1) {
            free(line);
            line = NULL;
            break;
        }
        int len = strlen(line);
        int i = 0;
        char c = line[i];
        if (is_comment) {
            //printf("zashel\n");
            print_info_file(file_out, cond_met_comment, NULL);
            while(c != '}' && i < len) {
                c = line[i];
                i++;
            }
            if (c != '}' || i == len) {
                //printf("srabotalo");
                free(line);
                line = NULL;
                continue;
            } else {
                is_comment = false;
            }
        }
        while(isspace(c)) {
            c = line[i];
            i++;
        }
        //printf("111\n");
        int index = 0;
        Vector* vet_st = NULL;
        Vector* vet_end = NULL;
        status_code st_act;
        //printf("%s\n", line);
        char* cmd = strstr(line, "read");
        if (cmd) {
            int check_len = cmd - line;
            //printf("ZDES COMMEN\n");
            for (;i < check_len; i++) {
                if (line[i] == '%') {
                    print_info_file(file_out, cond_met_comment, NULL);
                    free(line);
                    line = NULL;
                    break;
                }
                if (line[i] == '{') {
                    print_info_file(file_out, cond_met_comment, NULL);
                    is_comment = true;
                    while (line[i] != '}' && i < len) {
                        i++;
                    }
                    if (line[i] == '}') {
                        is_comment = false;
                    } else {
                        free(line);
                        line = NULL;
                        break;
                    }

                }
            }
            if (!line) continue;
            cmd += 4;
            int base;
            //printf("%s\n", cmd);
            if (sscanf(cmd, "(%c, %d)", &c, &base)!= 2) {
                print_info_file(file_out, cond_error_reading, NULL);
                free(line);
                line = NULL;
                continue;
            }
            //printf("%c, %d\n", c, base);
            vet_st = find_vector(storage, *size_storage, c);
            if (!vet_st) {
                st_act = create_vector(&vet_st, c);
                if (st_act != code_success) {
                    free(line);
                    line = NULL;
                    fclose(file);
                    if (file_out) fclose(file_out);
                    return st_act;
                }
                add_to_storage(storage, vet_st, size_storage);
            }
            st_act = read_vector(&vet_st, base);
            if (st_act != code_success) {
                free(line);
                line = NULL;
                // ne prochital
                print_info_file(file_out, cond_error_reading, NULL);
                if (st_act == code_error_alloc) {
                    return st_act;
                }
                continue;
            }
            while (i < len) {
                c = line[i];
                i++;
                if (line[i] == '{') {
                    print_info_file(file_out, cond_met_comment, NULL);
                    is_comment = true;
                    while (line[i] != '}' && i < len) {
                        i++;
                    }
                    if (line[i] == '}') {
                        is_comment = false;
                    } else {
                        free(line);
                        line = NULL;
                        break;
                    }

                }
            }
            free(line);
            line = NULL;
            continue;
        } else {
            int check_len = cmd - line;
            cmd = strstr(line, "write");
            //printf("zdes\n");
            if (cmd) {
                for (;i < check_len; i++) {
                    if (line[i] == '%') {
                        print_info_file(file_out, cond_met_comment, NULL);
                        free(line);
                        line = NULL;
                        break;
                    }
                    if (line[i] == '{') {
                        print_info_file(file_out, cond_met_comment, NULL);
                        is_comment = true;
                        while (line[i] != '}' && i < len) {
                            i++;
                        }
                        if (line[i] == '}') {
                            is_comment = false;
                        } else {
                            free(line);
                            line = NULL;
                            break;
                        }

                    }
                }
                if (!line) continue;
                cmd += 5;
                int base;
                    //printf("%s---\n", cmd);
                if (sscanf(cmd, "(%c, %d)", &c, &base)!= 2) {
                    free(line);
                    line = NULL;
                    //printf("neee\n");
                    print_info_file(file_out, cond_error_reading, NULL);
                    continue;
                        // ne prochitalo
                }
                vet_st = find_vector(storage, *size_storage, c);
                //printf("%c>>>\n", vet_st->name);
                if (!vet_st) {
                    printf("pustoy\n");
                }
                if (!vet_st) {
                    print_info_file(file_out, cond_unknown_var, NULL);
                    free(line);
                    line = NULL;
                    // printf("ne nashlo\n");
                    continue;
                        // ne nashlo
                }
                    //printf("ndasdasdo\n");
                st_act = print_vector(vet_st, base, output);
                //print_info_file(stdout, cond_value, vet_st);
                if (st_act != code_success) {
                    //printf("tttuuut\n");
                    print_info_file(file_out, cond_error_writing, NULL);
                    free(line);
                    line = NULL;
                    continue;
                    if (st_act == code_error_alloc) {
                        return st_act;
                    }
                }
                free(line);
                line = NULL;
                continue;
            }
        }
        
        if (!line) continue;
        if (c == '{') {
            is_comment = true;
            print_info_file(file_out, cond_met_comment, NULL);
            while (c != '}' && i < len) {
                c = line[i];
                i++;
            }
            if (c == '}') {
                is_comment = false;
            } else {
                free(line);
                line = NULL;
                continue;
            }
        }
        //printf("222\n");


        if (isalpha(c) && !is_comment) {
            vet_st = find_vector(storage, *size_storage, c);
            //printf("NASHEL %c\n", c);
            if (!vet_st) {
                //printf(" ne NASHEL %c\n", c);
                st_act = create_vector(&vet_st, c);
                if (st_act != code_success) {
                    free(line);
                    fclose(file);
                    if (file_out) fclose(file_out);
                    return st_act;
                }
                add_to_storage(storage, vet_st, size_storage);
                printf("DOBAVIL\n");
            }
        }
        c = line[i];
        i++;
        index = 0;
        while(isspace(c)) {
            c = line[i];
            i++;
        }   
        //printf("333\n");
        char operator[256];
        index = 0;
        //printf("%d -- %d %d\n", i, len, is_comment);
        if (i >= len) {
            free(line);
            line = NULL;
            continue;
        }
        Vector* summator = NULL;
        st_act = copy_vector(vet_st, &summator);
        if (st_act != code_success) {
            free(line);
            line = NULL;
            return st_act;
        }
        //printf("33333\n");
        for (;i < len; i++) {
            c = line[i];
            //printf("%c--fsdfsd-\n", c);
            if (isspace(c)) {
                //printf("here2\n");
                continue;
            } else if (c == '{' || c == '%') {
                if (c == '{') {
                    print_info_file(file_out, cond_met_comment, NULL);
                    is_comment = true;
                    while (c != '}' && i < len) {
                        c = line[i];
                        i++;
                    }
                    if (c == '}') {
                        is_comment = false;
                    } else {
                        
                        free(line);
                        line = NULL;
                    }
                } else {
                    print_info_file(file_out, cond_met_comment, NULL);
                    free(line);
                    line = NULL; 
                    i = len;
                    continue;
                }
            }
            else if (is_operator(c) && !isspace(c)) {
                operator[index] = c;
                //printf("here\n");
                index++;
            } else if (isalpha(c) && !isspace(c)) {
                if (index != 0) {
                    operator[index] = '\0';
                }
                index = 0;
                //printf("%s op\n", operator);
                //printf("%c\n", c);
                vet_end = find_vector(storage, *size_storage, c);
                st_act = operation(&summator, vet_end, operator);
                if (st_act != code_success) {
                    if (st_act == code_error_alloc) {
                        free(line);
                        line = NULL;
                        fclose(file);
                        if (file_out) {
                            fclose(file_out);
                        }
                        return code_error_alloc;
                    }
                    print_info_file(file_out, cond_invalid_parameter, NULL);
                    break;
                }
                //printf("%d size\n", summator->size);
                //print_vector(summator, 2, NULL);
                print_info_file(file_out, cond_value, summator);
                //printf("fsdfsdfsd\n");
                //print_vector(summator, 2, NULL);
                //printf("    fsdfsdfsd\n");
                //printf("SUMMMATOR\n");
                //print_vector(summator, 2, NULL);
                //printf("    SUMMMATOR\n");
                index = 0;
                operator[index] = '\0';
            }
            
        }
        
        for (int i = 0; i < summator->size; i++) {
            vet_st->data[i] = summator->data[i];
        }
        free_vector(summator);
        summator = NULL;
        //print_vector(vet_st, 2, NULL);
    }
    fclose(file);
    fclose(file_out);
    return code_success;
}

status_code copy_vector(Vector* a, Vector** b) {
    if (!a) return code_invalid_parameter;
    (*b) = (Vector*)malloc(sizeof(Vector));
    if (!*b) {
        return code_error_alloc;
    }
    (*b)->name = a->name;
    (*b)->capacity = a->capacity;
    (*b)->data = (char*)malloc(sizeof(char) * (*b)->capacity);
    (*b)->size = a->size;
    for (int i = 0; i < a->size; i++) {
        (*b)->data[i] = a->data[i];
    }
    return code_success;
}