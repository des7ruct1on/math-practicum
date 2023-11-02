#ifndef tree_h
#define tree_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef struct Node {
    char key;
    struct Node* child_main;
    struct Node* left_child;
    struct Node* right_child;
    struct Node* parent;
} Node;

typedef struct Tree {
    Node* root;
} Tree;

status_code insert_tree_main(Node* node, char data);
status_code insert_tree_right(Node* node, char data);
status_code scan_print_tree(FILE* input, FILE* output);
status_code build_tree(Tree* tree, char* expression);
void print_node(FILE* stream, Node* node, int depth);
void print_tree(FILE* stream, Tree* tree, char* expression);
void delete_tree(Node* node);

#endif