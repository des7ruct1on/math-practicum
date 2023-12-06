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

bool is_prime(int number) {
    if (number == 1 || number == 0) {
        return false;
    }
    for (int i = 1; i < (int)sqrt(number) + 1; i++) {
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
    do {
        number += 2;
    } while (!is_prime(number));
    return number;
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
    printf("%d wwwwwwwwwwww\n", *res);
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
        print_list(table->cells[i]);
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


status_code insert_table(Hash_table* table, char* key, char* value) {
    printf("--!!!!!!!!!!----\n");
    print_table(table);
    printf("-!!!!!!!!!!-\n");
    printf("    key: %s value: %s\n", key, value);
    Cell* to_add = NULL;
    status_code st_act;
    st_act = create_cell(&to_add, key, value);
    if (st_act != code_success) {
        free(to_add);
        return st_act;
    }
    printf("kkk %d\n", table->size);
    unsigned long long int index = hash(key) % table->size;
    printf("\thash %ld--\n", index);
    bool exist;
    st_act = is_exist(&exist, table->cells[index], to_add);
    if (st_act != code_success) {
        free_cell(to_add);
        return st_act;
    }
    to_add->key = index;
    printf("------\n");
    printf("found = %d\n", exist);
    if (!exist) {
        //if (!table->cells[index]) table->size++;
        printf("hueta %s %s\n", to_add->define, to_add->value);
        st_act = add_cell(&table->cells[index], to_add);
        if (st_act != code_success) {
            //printf("kek\n");
            free_cell(to_add);
            //table->size--;
            return st_act;
        }
        printf("size list: %d\n", size_list(table->cells[index]));
        printf("--fdsfdsdf----\n");
        free(key);
        key = NULL;
        free(value);
        value = NULL;
        printf("--zzzz----\n");
        printf("razmer %d\n", table->size);
        //print_list(table->cells[index]);
        //print_table(table);
        //printf("size: %d\n", size_list(table->cells[index]));
        //printf("%d<><><>\n", table->cells[index] == NULL);
        //printf("here\n");
        if (check_table_rebuild(table)) {
            printf("wow\n");
            printf("before\n");
            print_table(table);
            printf("-before\n");
            Hash_table* new_table = NULL;
            st_act = resize_table(table, &new_table);
            if (st_act != code_success) {
                free_cell(to_add);
                return st_act;
            }
            free_table(table);
            table = new_table;
            printf("after\n");
            print_table(table);
            printf("-after\n");
            printf("%d---cur_size\n", table->size);
        }
    }
    printf("after\n");
    print_table(table);
    printf("-after\n");
    printf("%d---cur_size\n", table->size);
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
            print_list(table->cells[i]);
        }

    }
}

status_code resize_table(Hash_table* old, Hash_table** new) {
    int new_size = get_next_prime(old->size);
    printf("next number: %d\n", new_size);
    status_code st_act;
    st_act = create_table(new, new_size);
    if (st_act != code_success) return st_act;

    for (int i = 0; i < old->size; i++) {
        if (old->cells[i]) {
            while (old->cells[i]) {
                char* _define = strdup(old->cells[i]->define);
                char* _value = strdup(old->cells[i]->value);
                st_act = insert_table((*new), _define, _value);
                if (st_act != code_success) {
                    free_table(*new);
                    free(_define);
                    free(_value);
                }
                old->cells[i] = old->cells[i]->next;
            }
        }
    }
    printf("======================================\n");
    print_table(*new);
    printf("======================================\n");
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