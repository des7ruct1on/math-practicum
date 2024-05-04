#ifndef trie_h
#define trie_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Trie {
    struct Trie* nodes[10];
    Post* office;
    int size;
} Trie;

status_code create_trie(Trie** trie);
status_code insert_trie(Logger* logger, Trie* root, char* key, Post* _office);
Post* search_trie(Trie* root, char* key);
bool has(Trie* root, const char* str);
void free_trie(Trie* trie, void(*free_storage)(void*));
void collect_keys_helper(Trie* node, char* buffer, int depth, char** keys, int* index);
char** get_keys(Trie* trie, int* size);
char* find_post_trie(Trie* trie, Post* find);
#endif