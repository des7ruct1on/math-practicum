#include "../headers/lab.h"
#include "../headers/string.h"

status_cmd command(char** arg_one, char** arg_two, char** arg_three, My_string** info) {
    char* cmd = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!cmd) {
        return cmd_error_alloc;
    }
    int index = 0;
    char symbol = getchar();
    while (!isspace(symbol)) {
        cmd[index++] = symbol;
        symbol = getchar();
    }
    cmd[index] = '\0';
    index = 0;
    if (!strcmp(cmd, "Exit")) {
        free(cmd);
        return cmd_exit;
    } else if (!strcmp(cmd, "Undo")) {
        free(cmd);
        return cmd_undo;
    } else if (!strcmp(cmd, "Print")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_print;
    } else if (!strcmp(cmd, "File")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_file;
    } else if (!strcmp(cmd, "Add")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_add;
    } else if (!strcmp(cmd, "Find")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        print_str(*info);
        free(*arg_one);
        return cmd_find;
    } else if (!strcmp(cmd, "Edit")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_two)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_two)[index] = '\0';
        index = 0;
        if (!strcmp(*arg_one, "surname")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_surname;
        } if (!strcmp(*arg_one, "name")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_name;
        } if (!strcmp(*arg_one, "last_name")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_last_name;
        } if (!strcmp(*arg_one, "birthday")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_date_birth;
        } if (!strcmp(*arg_one, "gender")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_gender;
        } if (!strcmp(*arg_one, "salary")) {
            free(*arg_one);
            (*info) = String(*arg_two);
            print_str(*info);
            //free(*arg_two);
            (*arg_three) = (char*)malloc(STR_SIZE * sizeof(char));
            if (*arg_three == NULL) {
                return cmd_error_alloc;
            }
            while(isspace(symbol)) {
                symbol = getchar();
            }
            while (symbol != '\n') {
                (*arg_three)[index++] = symbol;
                symbol = getchar();
            }
            (*arg_three)[index] = '\0';
            index = 0;
            return cmd_edit_salary;
        } else {
            free(*arg_one);
            free(*arg_two);
            return cmd_invalid_parameter;
        }
    } else if (!strcmp(cmd, "Remove")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        free(*arg_one);
        return cmd_remove;
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}


status_code stack_init(Stack** stack) {
    (*stack) = malloc(sizeof(Stack));
    if (!(*stack)) {
        return code_error_alloc;
    }
    (*stack)->size = 0;
    (*stack)->top = NULL;
    return code_success;
}

status_code stack_push(Stack* node, Action* act) {
    stack_node* tmp = (stack_node*)malloc(sizeof(stack_node));
    if (!tmp) {
        return code_error_alloc;
    }
    tmp->data = act;
    tmp->next = node->top;
    node->top = tmp;
    node->size++;
    return code_success;
}

void free_liver(Liver* tmp) {
    if (!tmp) {
        return;
    }
    if (tmp->surname->data) string_clear(tmp->surname); //free(tmp->surname);
    if (tmp->name->data) string_clear(tmp->name); //free(tmp->name);
    if (tmp->last_name->data) string_clear(tmp->last_name); //free(tmp->last_name);
    if (tmp->date_birth->data) string_clear(tmp->date_birth); //free(tmp->date_birth);
    if (tmp->gender->data) string_clear(tmp->gender); //free(tmp->gender);
    //free(tmp);
}

int free_action(Action* tmp) {
    if (!tmp) return -1;
    free_liver(tmp->condition);
    return 1;
}

Action* stack_pop(Stack* node) {
    if (node->size == 0) {
        return NULL;
    }
    stack_node* need = node->top;
    node->size--;
    node->top = need->next;
    Action* need_liver = need->data;
    free(need);
    return need_liver;
}

void stack_destroy(Stack* stack) {
    if (!stack) {
        return;
    }
    int _size = stack->size;
    if (_size == 0) {
        return;
    }
    for (int i = 0; i < _size; i++) {
        Action* tmp = stack_pop(stack);
        free_action(tmp);
        free(tmp);
    }
}

status_code list_init(List_node** list) {
    (*list) = malloc(sizeof(List_node));
    if (!(*list)) {
        return code_error_alloc;
    }
    return code_success;
}

status_code make_action(Liver* citizen, status_cmd _command, Action** tmp) {
    (*tmp) = (Action*)malloc(sizeof(Action));
    if (!(*tmp)) {
        return code_error_alloc;
    }
    (*tmp)->command = _command;
    (*tmp)->condition = citizen;
    return code_success;

}

void get_date(My_string* tmp, int* day, int* month, int* year) {
    if (!tmp) {
        return;
    }
    sscanf(tmp->data, "%d:%d:%d", day, month, year);
}

int compare_time(const My_string* a, const My_string* b) {
    int l_day, l_month, l_year;
    int r_day, r_month, r_year;
    if (sscanf(a->data, "%d:%d:%d", &l_day, &l_month, &l_year) != 3 ||
        sscanf(b->data, "%d:%d:%d", &r_day, &r_month, &r_year) != 3) {
        
        return 0;
    }

    int result = 0;

    if ((result = l_year - r_year) != 0) {
        return result;
    }

    if ((result = l_month - r_month) != 0) {
        return result;
    }

    return l_day - r_day;
}

status_code push_list(List_node** list, Liver* citizen, Stack* stack) {
    if (!citizen || !stack) {
        return code_invalid_parameter;
    }
    List_node* new = malloc(sizeof(List_node));
    if (!new) {
        return code_error_alloc;
    }
    status_code st_push;
    if ((*list) == NULL) {
        *list = new;
        (*list)->data = citizen;
        (*list)->next = NULL;
        Action* now = NULL;
        st_push = make_action(citizen, cmd_add, &now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        st_push = stack_push(stack, now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        //print_liver((*list)->data);
        return code_success;
    } else if ((*list)->next == NULL) {
        List_node* new_next = *list;
        *list = new;
        (*list)->data = citizen;
        (*list)->next = new_next;
        Action* now = NULL;
        st_push = make_action(citizen, cmd_add, &now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        st_push = stack_push(stack, now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        //print_liver((*list)->data);
        return code_success;
    } else {
        List_node* cur = *list;
        int _index = 0;
        int check;
        while (cur->next != NULL) {
            check = compare_time(citizen->date_birth, cur->next->data->date_birth);
            if (check < 0) {
                break;
            }
            _index++;
            cur = cur->next;
        }
        List_node* new_next = cur->next;
        cur->next = new;
        new->next = new_next;
        new->data = citizen;
        //print_liver(new->data);
        Action* now = NULL;
        st_push = make_action(citizen, cmd_add, &now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        now->index = _index;
        st_push = stack_push(stack, now);
        if (st_push == code_error_alloc) {
            return code_error_alloc;
        }
        return code_success;
    }
    
}

bool check_valid(char* check, int size) {
    if (!check) return true;
    for (int i = 0; i < size; i++) {
        if (!isalpha(check[i])) {
            return false;
        }
    }
    return true;
}

int compare_citizen(Liver* cur, char* _name, char* _surname, char* _last_name) {
    if (!cur) return 1;
    int diff_name, diff_surname, diff_last_name;
    diff_name = strcmp(cur->name->data, _name);
    diff_surname = strcmp(cur->surname->data, _surname);
    diff_last_name = 0;
    if (strcmp(_last_name, "")) diff_last_name = strcmp(cur->last_name->data, _last_name);
    if (diff_name == 0 && diff_surname == 0 && diff_last_name == 0) return 0;
    else return 1;
}

status_code remove_list(List_node** list, My_string* key, Stack* stack) {
    if (!(*list)) {
        return code_invalid_parameter;
    }
    print_str(key);
    status_code st_push;
    char* _key = key->data;
    char* surname = malloc(sizeof(char) * key->size);
    if (!surname) {
        return code_error_alloc;
    }
    char* name = malloc(sizeof(char) * key->size);
    if (!name) {
        free(surname);
        return code_error_alloc;
    }
    char* last_name = malloc(sizeof(char) * key->size);
    if (!last_name) {
        free(surname);
        free(name);
        return code_error_alloc;
    }
    int index = 0;
    status_free st_free;
    bool surname_f = false;
    bool name_f = false;
    bool last_name_f = false;
    int str_size = key->size;
    printf("str_size %d\n", str_size);
    for (int i = 0; i < str_size; i++) {
        if (!surname_f) {
            if (isspace(_key[i])) {
                surname[index] = '\0';
                index = 0;
                surname_f = true;
            } else {
                surname[index] = _key[i];
                index++;
                if (str_size == i + 1) {
                    surname[index] = '\0';
                    surname_f = true;
                }
            }
        } else {
            if (!name_f) {
                if (isspace(_key[i])) {
                    name[index] = '\0';
                    index = 0;
                    name_f = true;
                } else {
                    name[index] = _key[i];
                    index++;
                    if (str_size == i + 1) {
                        name[index] = '\0';
                        name_f = true;
                    }
                }
            } else {
                if (!last_name_f) {
                    if (isspace(_key[i])) {
                        last_name[index] = '\0';
                        index = 0;
                        last_name_f = true;
                    } else {
                        last_name[index] = _key[i];
                        index++;
                        if (str_size == i + 1) {
                            last_name[index] = '\0';
                            last_name_f = true;
                        }
                    }
                }
            }
        }
    }
    if (!surname_f || !name_f) {
        st_free = free_all(4, surname, name, last_name, key);
        return code_invalid_parameter;
    }
    if (!last_name_f) {
        last_name[0] = '\0';
    }
    int size_name = strlen(name);
    int size_surname = strlen(surname);
    int size_last_name = strlen(last_name);
    if (!check_valid(name, size_name) || !check_valid(surname, size_surname) || !check_valid(last_name, size_last_name)) {
        st_free = free_all(3, surname, name, last_name);
        return code_invalid_parameter;
    }
    List_node* cur = *list;
    int _index = 0;
    int check;
    while (cur->next!= NULL) {
        check = compare_citizen(cur->next->data, name, surname, last_name);
        if (check == 0) {
            break;
        }
        _index++;
        cur = cur->next;
    }
    if (check == 1) {
        st_free = free_all(3, surname, name, last_name);
        return code_invalid_parameter;
    }
    if (cur->next == NULL) {
        check = compare_citizen(cur->data, name, surname, last_name);
        if (check == 0) {
            Liver* deleted_citizen = cur->data;
            free(*list);
            *list = NULL;
            Action* now = NULL;
            st_push = make_action(deleted_citizen, cmd_remove, &now);
            if (st_push == code_error_alloc) {
                return code_error_alloc;
            }
            print_liver(deleted_citizen);
            now->index = _index;
            st_push = stack_push(stack, now);
            if (st_push == code_error_alloc) {
                return code_error_alloc;
            }
            st_free = free_all(3, surname, name, last_name);
            return code_success;
        } else {
            st_free = free_all(3, surname, name, last_name);
            return code_invalid_parameter;
        }
        
    }
    List_node* delete = cur->next;
    Liver* deleted_citizen = delete->data;
    List_node* new_next = delete->next;
    cur->next = new_next;
    Action* now = NULL;
    st_push = make_action(deleted_citizen, cmd_remove, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    free(delete);
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_free = free_all(3, surname, name, last_name);
    
    return code_success;
}

status_code find_citizen(List_node* list, My_string* key, Liver** find, int* _index) {
    if (!list) {
        return code_invalid_parameter;
    }
    status_code st_push;
    char* _key = strdup(key->data);
    char* surname = malloc(sizeof(char) * key->size);
    if (!surname) {
        return code_error_alloc;
    }
    char* name = malloc(sizeof(char) * key->size);
    if (!name) {
        free(surname);
        return code_error_alloc;
    }
    char* last_name = malloc(sizeof(char) * key->size);
    if (!last_name) {
        free(surname);
        free(name);
        return code_error_alloc;
    }
    int index = 0;
    status_free st_free;
    bool surname_f = false;
    bool name_f = false;
    bool last_name_f = false;
    int str_size = key->size;
    for (int i = 0; i < str_size; i++) {
        if (!surname_f) {
            if (isspace(_key[i])) {
                surname[index] = '\0';
                index = 0;
                surname_f = true;
            } else {
                surname[index] = _key[i];
                index++;
                if (str_size == i + 1) {
                    surname[index] = '\0';
                    surname_f = true;
                }
            }
        } else {
            if (!name_f) {
                if (isspace(_key[i])) {
                    name[index] = '\0';
                    index = 0;
                    name_f = true;
                } else {
                    name[index] = _key[i];
                    index++;
                    if (str_size == i + 1) {
                        name[index] = '\0';
                        name_f = true;
                    }
                }
            } else {
                if (!last_name_f) {
                    if (isspace(_key[i])) {
                        last_name[index] = '\0';
                        index = 0;
                        last_name_f = true;
                    } else {
                        last_name[index] = _key[i];
                        index++;
                        if (str_size == i + 1) {
                            last_name[index] = '\0';
                            last_name_f = true;
                        }
                    }
                }
            }
        }
    }
    if (!surname_f || !name_f) {
        st_free = free_all(4, surname, name, last_name, key);
        return code_invalid_parameter;
    }
    if (!last_name_f) {
        last_name[0] = '\0';
    }
    int size_name = strlen(name);
    int size_surname = strlen(surname);
    int size_last_name = strlen(last_name);
    if (!check_valid(name, size_name) || !check_valid(surname, size_surname)) {
        st_free = free_all(4, surname, name, last_name, key);
        *find = NULL;
        return code_invalid_parameter;
    }
    if (size_last_name) {
        if (!check_valid(last_name, size_last_name)) {
            st_free = free_all(4, surname, name, last_name, key);
            *find = NULL;
            return code_invalid_parameter;
        }
    }
    List_node* cur = list;
    int check;
    int index_check = 0;
    while (cur->next != NULL) {
        check = compare_citizen(cur->data, name, surname, last_name);
        if (check == 0) {
            break;
        }
        index_check++;
        cur = cur->next;
    }
    if (cur->next == NULL) {
        check = compare_citizen(cur->data, name, surname, last_name);
        if (check == 0) {
            *find = cur->data;
            *_index = index_check;
        } else {
            *find = NULL;
            *_index = -1;
        }
    } else {
        *find = cur->data;
        *_index = index_check;
    } 
    st_free = free_all(3, surname, name, last_name);
    return code_success;
}

status_code liver_copy(Liver** citizen, Liver* prev) {
    if (!prev) {
        return code_invalid_parameter;
    }
    (*citizen) = malloc(sizeof(Liver));
    if (!(*citizen)) {
        return code_error_alloc;
    }
    (*citizen)->surname = my_strcpy_new(prev->surname);
    (*citizen)->name = my_strcpy_new(prev->name);
    (*citizen)->last_name = my_strcpy_new(prev->last_name);
    (*citizen)->date_birth = my_strcpy_new(prev->date_birth);
    (*citizen)->gender = my_strcpy_new(prev->gender);
    (*citizen)->salary = prev->salary;
    return code_success;
}

status_code edit_surname(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_surname, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    string_clear(citizen->surname);
    citizen->surname = NULL;
    citizen->surname = my_strcpy_new(info);
    return code_success;
}

status_code edit_name(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_name, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    string_clear(citizen->name);
    citizen->name = NULL;
    citizen->name = my_strcpy_new(info);
    print_str(citizen->name);
    return code_success;
}

status_code edit_last_name(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_last_name, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    string_clear(citizen->last_name);
    citizen->last_name = NULL;
    citizen->last_name = my_strcpy_new(info);
    return code_success;
}

status_code edit_date_birth(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_date_birth, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    string_clear(citizen->date_birth);
    citizen->date_birth = NULL;
    citizen->date_birth = my_strcpy_new(info);
    return code_success;
}

status_code edit_gender(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_gender, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    string_clear(citizen->gender);
    citizen->gender = NULL;
    citizen->gender = my_strcpy_new(info);
    return code_success;
}

status_code edit_salary(Liver* citizen, My_string* info, int _index, Stack* stack) {
    if (!citizen || !info) {
        return code_invalid_parameter;
    }
    status_code st_push;
    Action* now = NULL;
    Liver* tmp = NULL;
    st_push = liver_copy(&tmp, citizen);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    st_push = make_action(tmp, cmd_edit_salary, &now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    now->index = _index;
    st_push = stack_push(stack, now);
    if (st_push == code_error_alloc) {
        return code_error_alloc;
    }
    double new_salary;
    sscanf(info->data, "%lf", &new_salary);
    citizen->salary = new_salary;
    return code_success;
}

Liver* find_index(List_node* list, int index) {
    if (!list || index < 0) {
        return NULL;
    }
    List_node* cur = list;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    return cur->data;
}

status_code undo(List_node* list, Stack* stack) {
    if (!stack) return code_invalid_parameter;
    Stack* tmp = NULL;
    status_code st_act;
    st_act = stack_init(&tmp);
    if (st_act == code_error_alloc) {
        return code_error_alloc;
    }
    My_string* sep = String(" ");
    int size = stack->size;
    if (size == 1) size++;
    for (int i = 0; i < size / 2; i++) {
        Action* now = stack_pop(stack);

        status_cmd was = now->command;
        Liver* cur = now->condition;
        printf("xxxx\n");

        My_string* info = NULL;;
        My_string* info_delete = NULL;
        int check;
        if (was == cmd_add) {
            info = my_strcpy_new(cur->surname);
            check = concat_strings(&info, sep);
            check = concat_strings(&info, cur->name);
            if (cur->last_name->size != 0) {
                check = concat_strings(&info, sep);
                check = concat_strings(&info, cur->last_name);
            }
            st_act = remove_list(&list, info, tmp);
            if (st_act != code_success) {
                stack_destroy(tmp);
                string_clear(info);
                free(info);
                return st_act;
            }
        } else if (was == cmd_remove) {
            info = my_strcpy_new(cur->surname);
            check = concat_strings(&info, sep);
            check = concat_strings(&info, cur->name);
            if (cur->last_name->size != 0) {
                check = concat_strings(&info, sep);
                check = concat_strings(&info, cur->last_name);
            }
            st_act = push_list(&list, cur, tmp);
            if (st_act != code_success) {
                stack_destroy(tmp);
                string_clear(info);
                free(info);
                return st_act;
            }
        } else if (was == cmd_edit_surname) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            string_clear(change->surname);
            free(change->surname);
            change->surname = my_strcpy_new(cur->surname);
        } else if (was == cmd_edit_name) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            string_clear(change->name);
            free(change->name);
            change->name = my_strcpy_new(cur->name);
        } else if (was == cmd_edit_last_name) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            string_clear(change->last_name);
            free(change->last_name);
            change->last_name = my_strcpy_new(cur->last_name);
        } else if (was == cmd_edit_date_birth) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            string_clear(change->date_birth);
            free(change->date_birth);
            change->date_birth = my_strcpy_new(cur->date_birth);
        } else if (was == cmd_edit_gender) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            string_clear(change->gender);
            free(change->gender);
            change->gender = my_strcpy_new(cur->gender);
        } else if (was == cmd_edit_salary) {
            Liver* change = find_index(list, now->index);
            if (change == NULL) {
                return code_invalid_parameter;
            }
            change->salary = cur->salary;
        }
        //free_action(now);
        free(now);
        string_clear(info);
        free(info);
        info = NULL;
    }
    string_clear(sep);
    free(sep);
    stack_destroy(tmp);
}

status_code print_file(const char* filename, List_node* list) {
    if (!list) return code_invalid_parameter;
    FILE* file = fopen(filename, "w");
    if (!file) {
        return code_error_oppening;
    }
    print_liver(list->data);
    List_node* current = list;
    fprintf(file, "| Surname | Name | Last name | Date birth | Gender | Salary |\n");
    fprintf(file, "|===========================================================|\n");
    while (current != NULL) {
        fprintf(file, "| %10s | %10s | %10s | %15s | %10s | %5lf |\n", current->data->surname->data, current->data->name->data, current->data->last_name->data, current->data->date_birth->data, current->data->gender->data, current->data->salary);
        current = current->next;
    }
    fprintf(file, "|===========================================================|\n");
    fclose(file);
    return code_success;
}

void print_liver(Liver* liver) {
    if (!liver) return;
    printf(">\n");
    printf("| Surname : %s\n", liver->surname->data);
    printf("| Name : %s\n", liver->name->data);
    printf("| Last name : %s\n", liver->last_name->data);
    printf("| Date birth : %s\n", liver->date_birth->data);
    printf("| Gender : %s\n", liver->gender->data);
    printf("| Salary : %lf\n", liver->salary);
    printf(">\n");
}

bool is_time(char* time, int size) {
    if (!time) {
        return false;
    }
    char symb = ':';
    int counter = 0;
    for (int i = 0; i < size; i++) {
        if (time[i] == symb) {
            counter++;
        }
    }
    if (counter == 2) {
        return true;
    } 
    return false;
}


int check_valid_gender(char* str) {
    if (!strcmp(str, "M") || !strcmp(str, "F")) {
        return 1;
    } else {
        return 0;
    }
}

status_code create_liver(char* info, Liver** tmp) {
    if (!info) {
        return code_invalid_parameter;
    }
    status_free st_free;
    (*tmp) = malloc(sizeof(Liver));
    if (!(*tmp)) {
        return code_error_alloc;
    }
    char* _surname = malloc(sizeof(char) * STR_SIZE);
    if (!_surname) {
        return code_error_alloc;
    }
    char* _name = malloc(sizeof(char) * STR_SIZE);
    if (!_name) {
        st_free = free_all(1, _surname);
        return code_error_alloc;
    }
    char* _gender = malloc(sizeof(char) * STR_SIZE);
    if (!_gender) {
        st_free = free_all(2, _surname, _name);
        return code_error_alloc;
    }
    char* _last_name = malloc(sizeof(char) * STR_SIZE);
    if (!_last_name) {
        st_free = free_all(3, _surname, _name, _gender);
        return code_error_alloc;
    }
    char* _date_birth = malloc(sizeof(char) * STR_SIZE);
    if (!_date_birth) {
        st_free = free_all(4, _surname, _name, _gender, _last_name);
        return code_error_alloc;
    }
    char* _number = malloc(sizeof(char) * STR_SIZE);
    if (!_date_birth) {
        st_free = free_all(5,  _surname, _name, _gender, _last_name, _number);
        return code_error_alloc;
    }
    if (sscanf(info, "%s %s %s %s %s %s", _surname, _name, _last_name, _date_birth, _gender, _number) < 5)  {
        st_free = free_all(7, *tmp, _surname, _name, _gender, _last_name, _date_birth, _number);
        return code_invalid_parameter;
    }
    int _size = strlen(_last_name);
    double num;
    if (is_time(_last_name, _size)) {
        (*tmp)->surname = String(_surname);
        (*tmp)->name = String(_name);
        (*tmp)->last_name = String("");
        (*tmp)->date_birth = String(_last_name);
        if (!check_valid_gender(_date_birth)) {
            st_free = free_all(7, *tmp, _surname, _name, _gender, _last_name, _date_birth, _number);
            return code_invalid_parameter;
        }
        (*tmp)->gender = String(_date_birth);
        num = atof(_gender);
        (*tmp)->salary = num;
    } else {
        (*tmp)->surname = String(_surname);
        (*tmp)->name = String(_name);
        (*tmp)->last_name = String(_last_name);
        (*tmp)->date_birth = String(_date_birth);
        if (!check_valid_gender(_gender)) {
            st_free = free_all(7, *tmp, _surname, _name, _gender, _last_name, _date_birth, _number);
            return code_invalid_parameter;
        }
        (*tmp)->gender = String(_gender);
        num = atof(_number);
        (*tmp)->salary = num;
    }
    st_free = free_all(6, _surname, _name, _gender, _last_name, _date_birth, _number);
    return code_success;
}

status_code read_from_file(const char* filename, List_node** storage, Stack* stack) {
    FILE* in = fopen(filename, "r");
    if (!in) {
        return code_error_oppening;
    }
    char* line = NULL;
    status_realloc st_realloc;
    int read;
    status_free st_free;
    My_string* info = NULL;
    status_code st_act;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        //printf("111\n");
        Liver* cur_liver = NULL;
        st_act = create_liver(line, &cur_liver);
        if (st_act != code_success) {
            free(line);
            return st_act;
        } 
        //printf("111\n");
        st_act = push_list(storage, cur_liver, stack);
        if (st_act == code_error_alloc || st_act == code_invalid_parameter) {
            free(line);
            return st_act;
        }
        //printf("111\n");
        free(line);
        line = NULL;
    }
    if (line) free(line);
    fclose(in);
    return code_success;
}

void free_list(List_node* list) {
    if (!list) return;
    List_node* tmp = list;
    while(tmp->next != NULL) {
        List_node* del = tmp->next;
        free_liver(tmp->data);
        free(tmp);
        tmp = del;
    }
    free(list);
}