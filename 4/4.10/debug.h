#ifndef _DEBUG_H_
#define _DEBUG_H_

#define _GNU_SOURCE

#include "Trie.h"
#include "operations.h"
#include <math.h>

status print_variables(Trie_ptr trie);
void print_memory_dump(void * ptr, size_t size);
status print_variable_and_value(Trie_ptr trie);
status change_variable_value(Trie_ptr trie);
status insert_new_variable(Trie_ptr trie, char ** var_name);
status zeckendorf_to_decimal(char* zeckendorf, uint32_t * res);
status insert_new_variable_zekendorf(Trie_ptr trie, char * name);
status roman_to_decimal(char* roman, uint32_t * res);
status insert_new_variable_rim(Trie_ptr trie, char * name);
status remove_variable(Trie_ptr trie, char * variables[], size_t * size);
status debugger(Trie_ptr trie, int * work_flag);
status add_to_names_array(char *** names, size_t * size, size_t * capacity, char * name);

#endif