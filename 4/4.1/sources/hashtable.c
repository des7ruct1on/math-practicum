#include "../headers/hashtable.h"

unsigned long long int hash(const char* key) {
    if (!key) return 0;
    int _size = strlen(key);
    unsigned long long int result = 0;
    int pow = 1;
    for (int i = 0; i < _size; i++) {
        if (key[i] == '\0') continue;
        result += pow * ((isdigit(key[i])) ? key[i] - '0' : ('A' <= key[i] && key[i] <= 'Z') ? key[i] - 'A' + 10 : key[i] - 'a' + 36);
        pow *= 62;
    } 
    return result;
}

status_code create_table(Hash_table** new, int _size) {
    (*new) = (Hash_table*)malloc(sizeof(Hash_table));
    if (!(*new)) return code_error_alloc;

    (*new)->size = _size;
    (*new)->cells = (Cell**)malloc(sizeof(Cell*) * _size);
    if (!(*new)->cells) {
        free(*new);
        free((*new)->cells);
        return code_error_alloc;
    }
    for (int i = 0; i < _size; i++) {
       (*new)->cells[i] = NULL;
    }
    return code_success;
}

bool is_prime(int number) {
    if (number == 1 || number == 0) {
        return false;
    }
    for (int i = 0; i < (int)sqrt(number) + 1; i++) {
        if (number % i == 0) {
            return true;
        }
    }
    return false;
}

int get_next_prime(int number) {
    if (number % 2 == 0) {
        number++;
    }
    while (!is_prime(number)) {
        number += 2;
    }
    return number;
}

status_code create_cell(Cell** new, const char* key, const char* value) {
    if (!key || !value) return code_invalid_parameter;
    (*new) = (Cell*)malloc(sizeof(Cell));
    if (!(*new)) {
        return code_error_alloc;
    }
    (*new)->define = strdup(key);
    if (!(*new)->define) return code_error_alloc;
    (*new)->value = strdup(value);
    if (!(*new)->value) return code_error_alloc;
    (*new)->next = NULL;
    return code_success;
}

status_code add_cell(Cell** head, Cell *item) {
    Cell *tmp = (Cell*)malloc(sizeof(Cell));
    if (!tmp) {
        return code_error_alloc;
    }
    tmp->value = strdup(item->value);
    tmp->next = (*head);
    if (!tmp->next) {
        tmp->size = 1;
    }
    else {
        tmp->size = (*head)->size + 1;
    }
    (*head) = tmp;
    return code_success;
}

status_code is_exist(bool *res, Cell* list, Cell* new) {
    if (!new) return code_invalid_parameter;
    if (!list) {
        *res = false;
        return code_success;
    }
    Cell* cur = list;
    char* cur_key = cur->define;
    char* cur_value = cur->value;
    while (cur != NULL) {
        cur_key = cur->define;
        if (!strcmp(cur_key, new->define)) {
            free(cur->define);
            cur->define = NULL;
            cur->define = strdup(new->define);
            if (!cur->define) return code_error_alloc;
            *res = true;
        }
        cur = cur->next;

    }
    *res = false;
    return code_success;
}


void free_cell(Cell* del) {
    if (!del) return;
    int _size = del->size;
    Cell* cur = del;
    while (cur != NULL) {
        Cell* to_del = cur->next;
        free(cur->define);
        free(cur->value);
        free(cur);
        cur = NULL;
        cur = to_del;
    }
    free(del);
}

bool check_table_rebuild(Hash_table* table) {
    int min = table->cells[0]->size;
    int max = table->cells[0]->size;

    for (int i = 1; i < table->size; i++) {
        if (table->cells[i] != NULL) {
            if (table->cells[i]->size > max) {
                max = table->cells[i]->size;
            }
            if (table->cells[i]->size < min) {
                min = table->cells[i]->size;
            }
        }
    }
    if (max / min >= 2) return true;
    return false;
}

status_code insert_table(Hash_table* table, const char* key, const char* value) {
    printf("key: %s\nvalue: %s\n", key, value);
    Cell* to_add = NULL;
    status_code st_act;
    st_act = create_cell(&to_add, key, value);
    if (st_act != code_success) {
        free(to_add);
        return st_act;
    }
    int index = hash(key) % table->size;
    printf("%d--\n", index);
    bool exist;
    st_act = is_exist(&exist, table->cells[index], to_add);
    if (st_act != code_success) {
        free_cell(to_add);
        return st_act;
    }

    if (!exist) {
        if (!table->cells[index]) table->size++;
        st_act = add_cell(&table->cells[index], to_add);
        if (st_act != code_success) {
            free_cell(to_add);
            table->size--;
            return st_act;
        }
        if (check_table_rebuild) {
            Hash_table* new = NULL;
        }
    }
    return code_success;
}

void free_table(Hash_table* table) {
    if (!table) return;
    for (int i = 0; i < table->size; i++) {
        free_cell(table->cells[i]);
    }
    free(table->cells);
    free(table);
}

void print_table(Hash_table* table) {
    for (int i = 0; i < table->size; i++) {
        Cell* cur = table->cells[i];
        if (!cur) {
            printf("hash: %d\n", i);
            Cell* current = cur;
            while (current != NULL) {
                printf("\t <%s> <%s>\n", current->define, current->value);
                current = current->next;
            }
        }

    }
}

status_code resize_table(Hash_table** new, Hash_table* old) {
    if (!old) return code_success;
    int new_size = get_next_prime(old->size);
    status_code st_act;
    st_act = create_table(new, new_size);
    if (st_act != code_success) {
        free_table(*new);
        return st_act;
    }
    for (int i = 0; i < old->size; i++) {
        Cell* cur_list = old->cells[i];
        if (cur_list != NULL) {
            Cell* current = cur_list;
            while (current != NULL) {
                st_act = insert_table(*new, current->define, current->value);
                if (st_act != code_success) {
                    free_table(*new);
                    return st_act;
                }
                current = current->next;
            }
        }
    }
    free_table(old);
    return code_success;
}

char* find_from_list(Cell* list, const char* key) {
    if (!list || !key) return NULL;
    Cell* cur = list;
    while (cur != NULL) {
        if (!strcmp(cur->define, key)) {
            return cur->value;
        }
        cur = cur->next;
    }
}

char* find_element(Hash_table* table, const char* key) {
    if (!key) return NULL;
    int index = hash(key) % table->size;
    return find_from_list(table->cells[index], key);
}