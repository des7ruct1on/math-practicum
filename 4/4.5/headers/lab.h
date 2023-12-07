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

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc,
    code_missed_left_bracket,
    code_missed_right_bracket
} status_code;

typedef struct Tree {
    char data;
    struct Tree* left;
    struct Tree* right;
} Tree;

typedef struct stack_node_char {
    char data;
    struct stack_node_char* next;
} stack_node_char;

typedef struct Stack_char {
    stack_node_char* top;
    int size;
} Stack_char;

typedef struct stack_node_num {
    long long data;
    struct stack_node_num* next;
} stack_node_num;

typedef struct Stack_num {
    stack_node_num* top;
    int size;
} Stack_num;

status_code create_tree(Tree** tree, char value);
status_code insert_tree(Tree** tree, char value);
void free_tree(Tree* tree);
status_code create_stack(Stack_char** stack);
bool is_empty_stack(Stack_char* st);
int size_stack(Stack_char* st);
char top_stack(Stack_char* st);
char pop_stack(Stack_char* st);
status_code push_stack(Stack_char* st, char _data);
void free_stack(Stack_char* st);
bool is_operator(char c);
int get_priority(char c);
int op_assoc(char op);
status_code to_pol_expression(char* expression, char** pol_expression);
status_code create_stack_num(Stack_num** stack);
bool is_empty_stack_num(Stack_num* st);
int size_stack_num(Stack_num* st);
long long top_stack_num(Stack_num* st);
long long pop_stack_num(Stack_num* st);
status_code push_stack_num(Stack_num* st, long long _data);
void free_stack_num(Stack_num* st);
double binary_pow(double base, int power);
status_code compute(char* postfix, long long* res);
#endif