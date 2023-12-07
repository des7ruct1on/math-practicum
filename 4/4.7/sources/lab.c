#include "../headers/lab.h"

status_code create_storage(Storage** st) {
    (*st) = (Storage*)malloc(sizeof(Storage));
    if (!(*st)) return code_error_alloc;
    (*st)->capacity = MIN_SIZE;
    (*st)->size = 0;
    (*st)->data = (MemoryCell*)malloc(sizeof(MemoryCell) * MIN_SIZE);
    if (!(*st)->data) {
        free(*st);
        return code_error_alloc;
    }
    return code_success;
}

status_code resize_storage(Storage** st) {
    if (!st) return code_invalid_parameter;

    MemoryCell* tmp = realloc((*st)->data, (*st)->capacity * 2);
    if (!tmp) {
        return code_error_alloc;
    } else {
        (*st)->data = tmp;
        (*st)->capacity *= 2;
    }
    return code_success;
}

void free_cell(MemoryCell* cell) {
    if (!cell) return;
    if (cell->var) {
        free(cell->var);
        cell->var = NULL;
    }
}

void free_storage(Storage* st) {
    if (!st) return;

    int size = st->size;
    if (st->data) {
        for (int i = 0; i < size; i++) {
            free_cell(&st->data[i]);
        }
        free(st->data);   
    }
    free(st);
    st = NULL;
}

status_code create_cell(MemoryCell** cell, char* name, int val) {
    if (!name) return code_invalid_parameter;

    (*cell) = (MemoryCell*)malloc(sizeof(MemoryCell));
    if (!(*cell)) {
        return code_error_alloc;
    }
    (*cell)->var = strdup(name);
    if (!(*cell)->var) {
        free(*cell);
        return code_error_alloc;
    }
    (*cell)->value = val;
    return code_success;
}

int compare_cells(const void* a, const void* b) {
    return strcmp(((MemoryCell*)a)->var, ((MemoryCell*)b)->var);
}

int get_value_index(Storage* st, int index) {
    if (!st) return -1;
    //printf("index to check %d\n", index);
    return st->data[index].value;
}

int binary_search(Storage* st, char* key) {
    int left = 0;
    int right = st->size - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        if (strcmp(st->data[mid].var, key) < 0) {
            left = mid + 1;
        } else if (strcmp(st->data[mid].var, key) > 0) {
            right = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}

status_code add_cell(Storage** st, char* name, int value) {
    if (!name) return code_invalid_parameter;
    int index = (*st)->size;
    MemoryCell* new = NULL;
    status_code st_act;
    //printf("suka %s\n", name);
    st_act = create_cell(&new, name, value);
    if (st_act != code_success) {
        return st_act;
    }
    //printf("        addd stroka %s\n", new->var);
    (*st)->data[index] = *new;
    if ((*st)->size + 1 == (*st)->capacity) {
        st_act = resize_storage(st);
        if (st_act != code_success) {
            free_cell(new);
            return st_act;
        }
    }
    (*st)->size++;
    qsort((*st)->data, (*st)->size, sizeof(MemoryCell), compare_cells);
    return code_success;
}

status_code edit_var(Storage* st, char* name, int new_val) {
    //printf("suka upper %s\n", name);
    int index = binary_search(st, name);
    if (index == -1) {
        return add_cell(&st, name, new_val);
    } else {
        st->data[index].value = new_val;
    }
    return code_success;
}

void print_storage(Storage* st) {
    if (!st) return;
    int size = st->size;
    for (int i = 0; i < size; i++) {
        printf("Key: %s, Value: %d\n", st->data[i].var, st->data[i].value);
    }
}

void print(Storage* st, char* name) {
    if (strcmp(name, "\0")) {
        int index = binary_search(st, name);
        //printf("%d---------------------------\n", index);
        if (index == -1) {
            return;
        }
        printf("Key: %s, Value: %d\n", st->data[index].var, st->data[index].value);
    } else {
        print_storage(st);
    }
}

Operator get_op(char symb) {
    switch (symb) {
        case '+': 
            return PLUS;
        case '-':
            return MINUS;
        case '*':
            return MULT;
        case '^':
            return POW;
        case '%':
            return MOD;    
        case '/':
            return DIV;  
        default:
            return INVALID;
    }
}

bool is_number_str(char* str) {
    if (!str) return false;
    int size = strlen(str);

    for (int i = 0; i < size; i++) {
        if (!isdigit(str[i]) && str[i] != '\0') {
            return false;
        }
    }
    return true;
}

status_code process_line(char* line, Storage* st) {
    if (!line) return code_success;

    int size = strlen(line);
    int index = 0;  
    //printf("SIZE %d\n", size);
    char* cmd = (char*)malloc(sizeof(char) * size);
    if (!cmd) return code_error_alloc;

    char* arg_one = (char*)malloc(sizeof(char) * size);
    if (!arg_one) {
        free(cmd);
        return code_error_alloc;
    }

    char* arg_two = (char*)malloc(sizeof(char) * size);
    if (!arg_two) {
        free(cmd);
        free(arg_one);
        return code_error_alloc;
    }
    char c;
    bool is_read_first = false;
    bool is_read_second = false;
    bool is_read_third = false;
    Operator action;
    status_code st_act;
    for (int i = 0; i < size; i++) {
        c = line[i];
        if (!isspace(c) && c != '\n' && c != '\r' && c != ';') {
            //printf("symbol %c\n", c);
            Operator op = get_op(c);
            if (!is_read_first) {
                if (c == '=') {
                    cmd[index] = '\0';
                    index = 0;
                    is_read_first = true;
                } else {
                    cmd[index] = c;
                    //printf("    cmd add %c\n", cmd[index]);
                    index++;
                }
            } 
            else if (is_read_first && !is_read_second) {
                if (c == '=') continue;
                if (op == INVALID) {
                    arg_one[index] = c;
                    //printf("zalupa %c\n", arg_one[index]);
                    index++;
                } else {
                    action = op;
                    arg_one[index] = '\0';
                    index = 0;
                    is_read_second = true;
                }
            }
            else if (is_read_first && is_read_second) {
                if (op == INVALID) {
                    arg_two[index] = c;
                    index++;
                } else {
                    arg_two[index] = '\0';
                    index = 0;
                    is_read_third = true;
                }
            }
            //printf("    %d index\n", index);
        } else if (isspace(c) && c != '\n' && c != '\r') {
            if (!is_read_first) {
                cmd[index] = '\0';
                index = 0;
                is_read_first = true;
            } 
        }
    }
    //printf("%d %d %d\n", is_read_first, is_read_second, is_read_third);
    if (!is_read_first) {
        cmd[index] = '\0';
        index = 0;
    }
    if (!is_read_second) {
        arg_one[index] = '\0';
        index = 0;
    }
    if (!is_read_third) arg_two[index] = '\0';
    //printf("command: %s %d\n", cmd, strlen(cmd));
    //printf("arg_one: %s %d\n", arg_one, strlen(arg_one));
    //printf("arg_two: %s %d\n", arg_two, strlen(arg_two));
    
    if (!strcmp(cmd, "print")) {
        print(st, arg_one);
    } else {
        if (!strcmp(arg_two, "\0")) {
            int _value;
            if (is_number_str(arg_one)) {
                _value = atof(arg_one);
                //printf("        numberrr %d\n", _value);
                st_act = edit_var(st, cmd, _value);
                if (st_act != code_success) {
                    free(cmd);
                    free(arg_one);
                    free(arg_two);
                    return st_act;
                }
            } else {
                int index = binary_search(st, arg_one);
                if (index == -1) {
                    free(cmd);
                    free(arg_one);
                    free(arg_two);
                    return code_invalid_parameter;
                }

                _value = get_value_index(st, index);
                st_act = edit_var(st, cmd, _value);
                if (st_act != code_success) {
                    free(cmd);
                    free(arg_one);
                    free(arg_two);
                    return st_act;
                }
            }
        } else {
            //printf("1111\n");
            int _value1, _value2;
            //print_storage(st);
            if (is_number_str(arg_one)) {
                _value1 = atof(arg_one);
                //printf("2222\n");
            } else {
                int index_find = binary_search(st, arg_one);
                //printf("%d--\n", index);
                if (index_find == -1) {
                    free(cmd);
                    free(arg_one);
                    free(arg_two);
                    return code_invalid_parameter;
                }
                _value1 = get_value_index(st, index_find);
                //printf("3333\n");
            }
            if (is_number_str(arg_two)) {
                _value2 = atof(arg_two);
                //printf("4444\n");
            } else {
                int index_find = binary_search(st, arg_two);
                if (index_find == -1) {
                    free(cmd);
                    free(arg_one);
                    free(arg_two);
                    return code_invalid_parameter;
                }
                _value2 = get_value_index(st, index_find);
                //printf("5555\n");
            }
            int result;
            //printf("%d -- %d\n", _value1, _value2);
            st_act = solve_equation(action, _value1, _value2, &result);
            if (st_act != code_success) {
                free(cmd);
                free(arg_one);
                free(arg_two);
                return st_act;
            }
            //printf("6666\n");
            printf("Result: %d\n", result);
            st_act = edit_var(st, cmd, result);
            if (st_act != code_success) {
                free(cmd);
                free(arg_one);
                free(arg_two);
                return st_act;
            }
        }
    }
    free(cmd);
    free(arg_one);
    free(arg_two);
    return code_success;
}

status_code solve_equation(Operator op, int a, int b, int* result) {
    if ((op == DIV || op == MOD) && b == 0) {
        return code_invalid_parameter;
    }
    if (op == PLUS) {
        *result = a + b;
    } else if (op == MINUS) {
        *result = a - b;
    } else if (op == MULT) {
        *result = a * b;
    } else if (op == DIV) {
        *result = a / b;
    } else if (op == POW) {
        if (a == 0 && b == 0) return code_invalid_parameter;
        *result = pow(a, b);
    } else if (op == MOD) {
        *result = a % b;
    } else {
        return code_invalid_parameter;
    }
    return code_success;
}

void print_error(status_code st) {
    switch (st) {
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
        case code_error_alloc:
            printf("Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            printf("Can`t open file!!!\n");
            break;
        default:
            break;
    }
}