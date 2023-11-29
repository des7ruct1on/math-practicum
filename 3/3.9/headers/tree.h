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
#define STR_SIZE 256

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_first,
    cmd_len,
    cmd_print,
    cmd_stat,
    cmd_find
} status_cmd;

typedef struct Node {
    struct Node* left;
    struct Node* right;
    char* word;
    int count;
} Node;

typedef struct List_tree {
    Node* element;
    struct List_tree* next;
} List_tree;

status_cmd command(char** arg_one);
void print_list(List_tree* list);
bool valid_char(char letter, char* seps[], int size);
status_code create_node(char* word, Node** node);
status_code insert(Node** root, char* word);
Node* search_in_tree(Node *root, char *word);
void free_tree(Node *root);
int compare_mets(Node* a, Node* b);
status_code push_list(List_tree** list, Node* add);
status_code add_nodes_to_list(Node* root, List_tree** Tree_list);
int size_list(List_tree* list);
void print_common_words(List_tree* list, int n);
void print_max_len_word(List_tree* list);
void print_least_len_word(List_tree* list);
void print_stats(Node *root);
int find_depth(Node* root);
status_code read_from_file(char* argv[], int argc, Node** Tree, List_tree** Tree_list);
int get_size_tree(Node* tree);
#endif