#ifndef lab_h
#define lab_h

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

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef struct Cell {
    char* define;
    char* value;
    unsigned long long int key;
    int size;
    struct Cell* next;
} Cell;

typedef struct Hash_table {
    Cell** cells;
    int size;
} Hash_table;

unsigned long long int hash(const char* key);
status_code create_table(Hash_table** new, int _size);
bool is_prime(int number);
int get_next_prime(int number);
status_code create_cell(Cell** new, const char* key, const char* value);
status_code add_cell(Cell** head, Cell *item);
status_code is_exist(bool *res, Cell* list, Cell* new);
void free_cell(Cell* del);
bool check_table_rebuild(Hash_table* table);
status_code insert_table(Hash_table* table, const char* key, const char* value);
void free_table(Hash_table* table);
status_code resize_table(Hash_table** new, Hash_table* old);
char* find_from_list(Cell* list, const char* key);
char* find_element(Hash_table* table, const char* key);
void print_table(Hash_table* table);
#endif