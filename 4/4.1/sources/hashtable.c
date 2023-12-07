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

void print_list(Cell* list) {
    if (!list) return;
    Cell* cur = list;
    while (cur != NULL) {
        printf("\tdefine: %s, value: %s\n", cur->define, cur->value);
        cur = cur->next;
    }
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

bool is_prime(const int num) {
    if (!num || num == 1) return false;
    if (num == 2) return true;
    if (!(num & 1)) return false;
    for (int i = 3; i * i <= num; i += 2) 
    {
        if (!(num % i)) return false;
    }
    return true;
}

int get_next_prime(int num) {
    if (!num || num == 1) return 2;
    if (!(num & 1)) return ++num;
    do {
        num += 2;
    } while (!is_prime(num));
    return num;
}

int size_list(Cell* list) {
    if (!list) return 0;
    Cell* cur = list;
    int size = 0;
    while (cur != NULL) {
        size++;
        cur = cur->next;
    }
    return size;
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

status_code add_cell(Cell** head, Cell* item) {
    Cell *tmp = (Cell*)malloc(sizeof(Cell));
    if (!tmp) {
        return code_error_alloc;
    }
    tmp->define = strdup(item->define);  
    tmp->value = strdup(item->value);  
    if (!tmp->value) {
        free(tmp);
        return code_error_alloc;
    }
    if (!(*head)) {
        *head = tmp;  
    } else {
        Cell* cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = tmp;
    }
    //printf("%d/???\n", size_list(*head));
    return code_success;
}

status_code is_exist(bool *res, Cell* list, Cell* new) {
    //printf("    %s stroka\n", new->define);
    if (!new) return code_invalid_parameter;
    if (!list) {
        *res = false;
        return code_success;
    }
    Cell* cur = list;
    char* cur_key = cur->define;
    char* cur_value = cur->value;
    //printf("%d\n", size_list(cur));
    while (cur != NULL) {
        cur_key = cur->define;
        //printf("str %s %s %d\n", cur_key, new->define, strcmp(cur_key, new->define));
        if (!strcmp(cur_key, new->define)) {
            free(cur->value);
            cur->value = NULL;
            cur->value = strdup(new->value);
            if (!cur->value) return code_error_alloc;
            *res = true;
            //printf("\t%s\n", cur->value);
            return code_success;
        }
        cur = cur->next;

    }
    *res = false;
    return code_success;
}


void free_cell(Cell* del) {
    if (!del) return;
    Cell* cur = del;
    while (cur != NULL) {
        Cell* to_del = cur->next;
        free(cur->define);
        cur->define = NULL;
        free(cur->value);
        cur->value = NULL;
        free(cur);
        cur = NULL;
        cur = to_del;
    }

}

bool check_table_rebuild(Hash_table* table) {
    int min = -1;
    int max = -1;
    int tmp_size = 0;
    for (int i = 0; i < table->size; i++) {
        //print_list(table->cells[i]);
        if (table->cells[i] != NULL) {
            tmp_size = size_list(table->cells[i]);
            if (tmp_size> max) {
                max = tmp_size;
            }
            if (tmp_size < min || min == -1) {
                min = tmp_size;
            }
        }
    }
    if (max / min >= 2) return true;
    return false;
}


status_code insert_table(Hash_table** table, char* key, char* value) {
    Cell* to_add = NULL;
    status_code st_act;
    st_act = create_cell(&to_add, key, value);
    if (st_act != code_success) {
        free(to_add);
        return st_act;
    }
    unsigned long long int index = hash(key) % (*table)->size;
    bool exist;
    st_act = is_exist(&exist, (*table)->cells[index], to_add);
    if (st_act != code_success) {
        free_cell(to_add);
        return st_act;
    }
    to_add->key = index;
    if (!exist) {
        st_act = add_cell(&(*table)->cells[index], to_add);
        if (st_act != code_success) {
            free_cell(to_add);
            return st_act;
        }
        free(key);
        key = NULL;
        free(value);
        value = NULL;
        free_cell(to_add);
        if (check_table_rebuild(*table)) {
            st_act = resize_table(table);
            if (st_act != code_success) {
                free_cell(to_add);
                return st_act;
            }
        }
    }

    return code_success;
}

void free_table(Hash_table* table) {
    if (!table) return;
    for (int i = 0; i < table->size; i++) {
        free_cell(table->cells[i]);
        table->cells[i] = NULL;
    }
    free(table->cells);
    free(table);
    table = NULL;
}

void print_table(Hash_table* table) {
    for (int i = 0; i < table->size; i++) {
        Cell* cur = table->cells[i];
        if (table->cells[i]) {
            printf("hash: %d\n", i);
            printf("%d--\n", size_list(table->cells[i]));
            print_list(table->cells[i]);
        }

    }
}

status_code resize_table(Hash_table** old) {
    int new_size = get_next_prime((*old)->size);
    //printf("next number: %d\n", new_size);
    status_code st_act;
    Hash_table* new = NULL;
    st_act = create_table(&new, new_size);
    if (st_act != code_success) return st_act;

    for (int i = 0; i < (*old)->size; i++) {
        if ((*old)->cells[i]) {
            while ((*old)->cells[i]) {
                char* _define = strdup((*old)->cells[i]->define);
                char* _value = strdup((*old)->cells[i]->value);
                st_act = insert_table(&new, _define, _value);
                if (st_act != code_success) {
                    free_table(new);
                    free(_define);
                    free(_value);
                }
                (*old)->cells[i] = (*old)->cells[i]->next;
            }
        }
    }
   // printf("======================================\n");
    //print_table(new);
    //printf("======================================\n");
    free_table(*old);
    *old = NULL;
    *old = new;
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
    return NULL;
}

char* find_element(Hash_table* table, const char* key) {
    if (!key) return NULL;
    int index = hash(key) % table->size;
    return find_from_list(table->cells[index], key);
}