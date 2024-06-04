#include "headers/hashset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/heaps/headers/binaryheap.h"
#include "../../include/heaps/headers/binomial.h"
#include "../../include/heaps/headers/fib.h"
#include "../../include/heaps/headers/leftistheap.h"
#include "../../include/heaps/headers/skew.h"
#include "../../include/heaps/headers/treap.h"
#include "../../include/maps/headers/array.h"
#include "../../include/maps/headers/bst.h"
#include "../../include/maps/headers/hashset.h"
#include "../../include/maps/headers/trie.h"

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

status_code create_cell(Cell** new, char* key, Post* value) {
    if (!key || !value) return code_invalid_parameter;
    (*new) = (Cell*)malloc(sizeof(Cell));
    if (!(*new)) {
        return code_error_alloc;
    }
    (*new)->define = strdup(key);
    if (!(*new)->define) return code_error_alloc;
    (*new)->value = value;
    (*new)->next = NULL;
    return code_success;
}

status_code add_cell(Cell** head, Cell* item) {
    Cell *tmp = (Cell*)malloc(sizeof(Cell));
    if (!tmp) {
        return code_error_alloc;
    }
    tmp->define = strdup(item->define);  
    tmp->value = item->value;  
    if (!(*head)) {
        *head = tmp;  
    } else {
        Cell* cur = *head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = tmp;
    }
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
    //Post* cur_value = cur->value;
    //printf("%d\n", size_list(cur));
    while (cur != NULL) {
        cur_key = cur->define;
        //printf("str %s %s %d\n", cur_key, new->define, strcmp(cur_key, new->define));
        if (!strcmp(cur_key, new->define)) {
            free(cur->value);
            cur->value = NULL;
            cur->value = new->value;
            *res = true;
            //printf("\t%s\n", cur->value);
            return code_success;
        }
        cur = cur->next;

    }
    *res = false;
    return code_success;
}


void free_cell(Cell* del, Heap type) {
    if (!del) return;
    Cell* cur = del;
    while (cur != NULL) {
        Cell* to_del = cur->next;
        free(cur->define);
        cur->define = NULL;
        switch(type) {
            case binary:
                free_binary((Binary_heap*)cur->value->storage);
                break;
            case binomial:
                free_binom_heap((Binomial_heap*)cur->value->storage);
                break;
            case fibonacci:
                free_fib_heap((Fibbonacci_heap*)cur->value->storage);
                break;
            case skew:
                free_skew((Skew_heap*)cur->value->storage);
                break;
            case leftist:
                free_leftist((Leftist_heap*)cur->value->storage);
                break;
            case treap:
                free_treap((Treap*)cur->value->storage);
                break;
        }
        free(cur->value->ops);
        free(cur);
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


status_code insert_table(Model* model, Logger* logger, Hash_table** table, char* key, Post* value) {
   // printf("%s---\n", value);
    Cell* to_add = NULL;
    status_code st_act;
    st_act = create_cell(&to_add, key, value);
    if (st_act != code_success) {
        create_log(&logger, "error after creating cell, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(to_add);
        return st_act;
    }
    unsigned long long int index = hash(key) % (*table)->size;
    bool exist;
    st_act = is_exist(&exist, (*table)->cells[index], to_add);
    if (st_act != code_success) {
        create_log(&logger, "error after checking is existing, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free_cell(to_add, model->heap_type);
        return st_act;
    }
    to_add->key = index;
    if (!exist) {
        //printf("zdws\n");
        st_act = add_cell(&(*table)->cells[index], to_add);
        if (st_act != code_success) {
            create_log(&logger, "error after adding cell, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            free_cell(to_add, model->heap_type);
            return st_act;
        }
        free(key);
        key = NULL;
        free(value);
        value = NULL;
        free_cell(to_add, model->heap_type);
        if (check_table_rebuild(*table)) {
            st_act = resize_table(model, logger, table, model->heap_type);
            if (st_act != code_success) {
                create_log(&logger, "error after resizing cell, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_cell(to_add, model->heap_type);
                return st_act;
            }
        }
       // print_table(*table);
    }

    return code_success;
}

void free_table(Hash_table* table, Heap type) {
    if (!table) return;
    for (int i = 0; i < table->size; i++) {
        free_cell(table->cells[i], type);
        table->cells[i] = NULL;
    }
    free(table->cells);
    free(table);
    table = NULL;
}

status_code resize_table(Model* model, Logger* logger, Hash_table** old, Heap type) {
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
                Post* _value = (*old)->cells[i]->value;
                st_act = insert_table(model, logger, &new, _define, _value);
                if (st_act != code_success) {
                    free_table(new, model->heap_type);
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
    free_table(*old, model->heap_type);
    *old = NULL;
    *old = new;
    return code_success;
}

Post* find_from_list(Cell* list, const char* key) {
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

Post* find_element(Hash_table* table, const char* key) {
    if (!key) return NULL;
    int index = hash(key) % table->size;
    return find_from_list(table->cells[index], key);
}

int table_size(Hash_table* table) {
    if (!table) return 0;
    int total_size = 0;
    for (int i = 0; i < table->size; i++) {
        total_size += size_list(table->cells[i]);
    }
    return total_size;
}

char* find_post_hashset(Hash_table* table, Post* find) {
    if (table == NULL || table->cells == NULL) {
        return NULL; 
    }

    for (int i = 0; i < table->size; i++) {
        Cell* current_cell = table->cells[i]; 
        while (current_cell != NULL) {
            if (current_cell->value == find) {
                return current_cell->define; 
            }
            current_cell = current_cell->next; 
        }
    }

    return NULL; 
}
