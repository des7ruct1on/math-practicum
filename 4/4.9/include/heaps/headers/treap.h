#ifndef treap_h
#define treap_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Treap {
    Request data;
    struct Treap* left;
    struct Treap* right;
} Treap;

Treap* merge_treap(Treap* a, Treap* b);
status_code treap_copy(Treap** copy, Treap* add);
void free_treap(Treap* tree);
Treap* merge_treap_copy(Treap* a, Treap* b);
void split_treap(Treap* root, Request* key, Treap** a, Treap** b);
Treap* create_node_treap(Request tmp);
void insert_treap(Treap** root, Request tmp);
Treap* search_treap(Treap* root, Request* tmp);
void erase_treap(Treap** root, Request* tmp);
void print_treap(Treap* root, int n);
int treap_size(Treap* root);
#endif

