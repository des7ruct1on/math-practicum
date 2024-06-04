#ifndef hashset_h
#define hashset_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define TABLE_SIZE 128
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
status_code create_cell(Cell** _new, char* key, Post* value);
status_code add_cell(Cell** head, Cell* item);
status_code is_exist(bool *res, Cell* list, Cell* _new);
void free_cell(Cell* del, Heap type);
bool check_table_rebuild(Hash_table* table);
status_code insert_table(Model* model, Logger* logger, Hash_table** table, char* key, Post* value);
void free_table(Hash_table* table, Heap type);
status_code resize_table(Model* model, Logger* logger, Hash_table** old, Heap type);
Post* find_from_list(Cell* list, const char* key);
Post* find_element(Hash_table* table, const char* key);
int table_size(Hash_table* table);
char* find_post_hashset(Hash_table* arr, Post* find);
#endif