#include "operations.h"

status convert_to_decimal_base(char * to_convert, int original_base, uint32_t * result)
{
    if (!to_convert) return invalid_function_argument;
    status error;
    if ((error = check_if_in_base(to_convert, original_base)) != success) return error;

    uint32_t preresult = 0;
    int size = strlen(to_convert);
    int power = 1;
    for (int i = size - 1; i >= 0; --i)
    {
        if (isdigit(to_convert[i])) preresult += (power) * (to_convert[i] - '0');
        else preresult += (power) * (toupper(to_convert[i]) - 'A' + 10);
        power *= original_base;
    }
    *result = preresult;
    return success;
}

status convert_to_base(uint32_t to_convert, int to_base, char ** result)
{
    if (to_base < 2 || to_base > 36) return invalid_base;
    int size = get_symbols_count(to_convert, to_base);
    *result = (char*)malloc((size + 1) * sizeof(char));
    if (!*result) return no_memory;
    (*result)[size] = 0;
    int i = size - 1;
    while (i >= 0)
    {
        int remainder = to_convert % to_base;
        if (remainder < 10) (*result)[i] = remainder + '0';
        else (*result)[i] = remainder - 10 + 'A';
        to_convert /= to_base;
        --i;
    }
    return success;
}

status get_value(Trie_ptr trie, char * name, uint32_t * result, int original_base)
{
    Trie_node_ptr node = NULL;
    uint32_t value;
    status error = Trie_find(trie, name, &node);

    if (error == success) value = node->value;
    else if (error == cannot_find && is_number(name) == success)
    {
        error = convert_to_decimal_base(name, original_base, &value);
        if (error != success) return error;
    }
    else return error;
    *result = value;
    return success;
}

status not_operation(Trie_ptr trie, uint32_t value, uint32_t * result)
{
    *result = ~value;
    return success;
}

status output_operation(Trie_ptr trie, uint32_t value, int output_base)
{
    if (output_base != 10)
    {
        char * result = NULL;
        status error;
        if ((error = convert_to_base(value, output_base, &result)) != success) return error;
        printf("%s\n", result);
        free(result);
    }
    else printf("%d\n", value);
    return success;
}

status input_operation(Trie_ptr trie, int input_base, uint32_t * result, char * name)
{
    char * line = NULL;
    size_t size = 0;
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    while (line[size - 1] == '\n') line[size - 1] = 0;
    status error;
    if ((error = convert_to_decimal_base(line, input_base, result)) != success)
    {
        free(line);
        return error;
    }
    if ((error = Trie_insert(trie, name, *result)) != success)
    {
        free(line);
        return error;
    }
    free(line);
    return success;
}

status add_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 + value_2;
    return success;
}

status mult_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 * value_2;
    return success;
}

status sub_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 - value_2;
    return success;
}

uint32_t fast_mod_pow(uint32_t base, uint32_t exponent, uint32_t modulus) 
{
    if (exponent == 0) return 1;
    uint32_t z = fast_mod_pow(base % modulus, exponent / 2, modulus) % modulus;
    if (exponent % 2 == 0) return (z * z) % modulus;
    return ((base % modulus) * ((z * z) % modulus)) % modulus;

}

status pow_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = fast_mod_pow(value_1, value_2, (1 << 31));
    return success;
}

status div_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    if (value_2 == 0) return invalid_lexeme;
    *result = value_1 / value_2;
    return success;
}

status rem_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    if (value_2 == 0) return invalid_lexeme;
    *result = value_1 % value_2;
    return success;
}

status xor_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 ^ value_2;
    return success;
}

status and_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 & value_2;
    return success;
}

status or_operation(Trie_ptr trie, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    *result = value_1 | value_2;
    return success;
}