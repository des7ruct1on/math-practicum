#ifndef _TRIE_H_
#define _TRIE_H_

#include "errors.h"
#include <string.h>
#include <ctype.h>

#define VARIABLE_CHARAS_COUNT (26*2 + 10 + 1) // letters, digits and "_"

typedef struct Trie_node
{
    struct Trie_node * childs[VARIABLE_CHARAS_COUNT];
    int is_final; // 1 - yes, 0 - no
    uint32_t value;
    
} Trie_node, *Trie_node_ptr;

typedef struct Trie
{
    Trie_node_ptr root;
} Trie, *Trie_ptr;

Trie_node_ptr Trie_node_create();
void Trie_node_free(Trie_node_ptr node);

Trie_ptr Trie_create();
void Trie_free(Trie_ptr tree);

int find_digits_count_in_number(int number);

status Trie_find(Trie_ptr tree, char * name, Trie_node_ptr * result);
status Trie_insert(Trie_ptr tree, char * name, uint32_t value);
status Trie_set_null(Trie_ptr tree);

#endif