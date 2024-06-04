#ifndef _TYPES_H_
#define _TYPES_H

#include <string.h>

#define OPERATIONS_COUNT 13

typedef enum
{
    NOT,
    INPUT,
    OUTPUT,
    ADD,
    MULT,
    SUB,
    POW,
    DIV,
    REM,
    XOR,
    AND,
    OR,
    ASSIGN
} operation;

typedef enum
{
    left,
    right,
    middle
} operation_syntax;

typedef enum
{
    unary,
    binary
} operation_type;

typedef struct current_settings
{
    char ** operations_names;
    operation_syntax * basic_syntax;
    operation_type * basic_types;
    operation_syntax operation_result_type;
    int debug;

} Current_settings, *Current_settings_ptr;

static char * start_operations_names[] = {"not", "input", "output", "add", "mult", "sub", "pow", "div", "rem", "xor", "and", "or", "="};

Current_settings_ptr create_current_settings(int debug);
void free_current_settings(Current_settings_ptr settings);
void change_name(Current_settings_ptr settings, operation operation_id, char * new_name);
void change_basic_syntax(Current_settings_ptr settings, operation operation_id, operation_syntax new_syntax);
void change_operation_result_type(Current_settings_ptr settings, operation_syntax new_res_type);

#endif