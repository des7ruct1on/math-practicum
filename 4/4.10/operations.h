#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

#include "errors.h"
#include "validation.h"
#include "Trie.h"

status convert_to_decimal_base(char * to_convert, int original_base, uint32_t * result);
status convert_to_base(uint32_t to_convert, int to_base, char ** result);

status get_value(Trie_ptr trie, char * name, uint32_t * result, int original_base);

status not_operation(Trie_ptr trie, uint32_t value, uint32_t * result);

status output_operation(Trie_ptr trie, uint32_t value, int ouput_base);

status input_operation(Trie_ptr trie, int input_base, uint32_t * result, char * name);

status add_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status mult_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status sub_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

uint32_t fast_mod_pow(uint32_t base, uint32_t exponent, uint32_t modulus);

status pow_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status div_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status rem_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status xor_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status and_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

status or_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result);

#endif