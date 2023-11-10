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
#include "string.h"

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_add,
    cmd_remove,
    cmd_find,
    cmd_edit_surname,
    cmd_edit_name,
    cmd_edit_last_name,
    cmd_edit_date_birth,
    cmd_edit_gender,
    cmd_edit_salary,
    cmd_print,
    cmd_undo,
    cmd_file
} status_cmd;

typedef struct Liver {
    My_string* surname;
    My_string* name;
    My_string* last_name;
    My_string* date_birth;
    My_string* gender;
    double salary;
} Liver;

typedef struct Action {
    status_cmd command;
    Liver* condition;
    int index;
} Action;


typedef struct List_node {
    Liver* data;
    struct List_node* next;
} List_node;

typedef struct stack_node {
    Action* data;
    struct stack_node* next;
} stack_node;

typedef struct Stack {
    stack_node* top;
    int size;
} Stack;

status_cmd command(char** arg_one, char** arg_two, My_string** info);
status_code stack_init(Stack** stack);
status_code stack_push(Stack* node, Action* act);
void free_liver(Liver* tmp);
int free_action(Action* tmp);
Action* stack_pop(Stack* node);
void stack_destroy(Stack* stack);
status_code list_init(List_node** list);
status_code make_action(Liver* citizen, status_cmd _command, Action** tmp);
void get_date(My_string* tmp, int* day, int* month, int* year);
int compare_time(const My_string* a, const My_string* b);
status_code push_list(List_node** list, Liver* citizen, Stack* stack);
bool check_valid(char* check, int size);
int compare_citizen(Liver* cur, char* _name, char* _surname, char* _last_name);
status_code remove_list(List_node* list, My_string* key, Stack* stack);
status_code find_citizen(List_node* list, My_string* key, Liver* find);
status_code liver_copy(Liver** citizen, Liver* prev);
status_code edit_surname(Liver* citizen, My_string* info, Stack* stack);
status_code edit_name(Liver* citizen, My_string* info, int _index, Stack* stack);
status_code edit_last_name(Liver* citizen, My_string* info, int _index, Stack* stack);
status_code edit_date_birth(Liver* citizen, My_string* info, int _index, Stack* stack);
status_code edit_gender(Liver* citizen, My_string* info, int _index, Stack* stack);
status_code edit_salary(Liver* citizen, My_string* info, int _index, Stack* stack);
Liver* find_index(List_node* list, int index);
status_code undo(List_node* list, Stack* stack);
status_code print_file(const char* filename, List_node* list);
void print_liver(Liver* liver);
bool is_time(char* time, int size);
status_code create_liver(char* info, Liver** tmp);
status_code read_from_file(const char* filename, List_node** storage, Stack* stack);
void free_list(List_node* list);
#endif