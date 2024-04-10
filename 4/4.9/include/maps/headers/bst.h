#ifndef bst_h
#define bst_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Tree {
    char* key;
    Post* post;
    struct Tree* left;
    struct Tree* right;
} Tree;

status_code create_tree(Tree** tree, char* key, Post* office);
status_code insert_tree(Tree** tree, char* key, Post* office);
void free_tree(Tree* root, void(*free_storage)(void*));
Post* search_bst(Tree* root, char* key);
int tree_size(Tree* root);

#endif