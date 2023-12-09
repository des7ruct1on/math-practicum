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

typedef struct stack_node {
    void* data;
    struct stack_node* next;
} stack_node;

typedef struct Stack {
    stack_node* top;
    int size;
} Stack;


status_code create_tree(Tree** tree, char value);
status_code insert_tree(Tree** tree, char value);
void free_tree(Tree* tree);
bool is_operator(char c);
int get_priority(char c);
int op_assoc(char op);
status_code to_pol_expression(char* expression, char** pol_expression);
double binary_pow(double base, int power);
status_code create_stack(Stack** stack);
bool is_empty_stack(Stack* st);
int size_stack(Stack* st);
void* top_stack(Stack* st);
void* pop_stack(Stack* st);
status_code push_stack(Stack* st, void* _data);
void free_stack(Stack* st);
status_code compute(char* postfix, long long* res);
#endif