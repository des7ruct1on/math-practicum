#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#define _GNU_SOURCE
#include "operations.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONFIG_FILE "config.txt"

status add_to_expression(char ** expression, char * to_add);
status make_unary_expression(Trie_ptr trie, char * variable_name, operation operation_name, uint32_t value, uint32_t * result, int input_base, int output_base, int assign_base);
status make_binary_expression(Trie_ptr trie, operation operation_name, uint32_t value_1, uint32_t value_2, uint32_t * result);
status solve_expression(Current_settings_ptr settings, Trie_ptr trie, char * expression, uint32_t * result, int input_base, int output_base, int assign_base);
status scan_buffer(Current_settings_ptr settings, Trie_ptr trie, char * buffer, int input_base, int output_base, int assign_base);

status free_from_delims(char ** string);
status my_strtok(char ** result, char ** st_string, const char * delim);
void shift_string(char ** string, int index);
status is_delim(char character, const char * delim);

status interpretate(FILE * file, Current_settings_ptr settings, int input_base, int assign_base, int output_base);

#endif