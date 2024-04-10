#ifndef hashset_h
#define hashset_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"
#define TABLE_SIZE 128


typedef struct Cell {
    char* define;
    Post* value;
    unsigned long long int key;
    struct Cell* next;
} Cell;

typedef struct Hash_table {
    Cell** cells;
    int size;
} Hash_table;

unsigned long long int hash(const char* key);
status_code create_table(Hash_table** _new, int _size);
bool is_prime(const int num);
int get_next_prime(int num);
int size_list(Cell* list);
status_code create_cell(Cell** _new, const char* key, const Post* value);
status_code add_cell(Cell** head, Cell* item);
status_code is_exist(bool *res, Cell* list, Cell* _new);
void free_cell(Cell* del, void(*free_storage)(void*));
bool check_table_rebuild(Hash_table* table);
status_code insert_table(Hash_table** table, char* key, Post* value, void(*free_storage)(void*));
void free_table(Hash_table* table, void(*free_storage)(void*));
status_code resize_table(Hash_table** old, void(*free_storage)(void*));
Post* find_from_list(Cell* list, const char* key);
Post* find_element(Hash_table* table, const char* key);
int table_size(Hash_table* table);
#endif