#include "validation.h"

status check_if_in_base(char * number, int base)
{
    if (base < 2 || base > 36) return invalid_base;
    int i = 0;
    while (number[i])
    {
        if (!isdigit(number[i]) && !isalpha(number[i])) return invalid_variable;
        else if (isdigit(number[i]) && number[i] - '0' >= base) return not_in_base;
        else if (isalpha(number[i]) && toupper(number[i]) - 'A' + 10 >= base) return not_in_base;
        ++i;
    }
    return success;
}

int get_symbols_count(uint32_t number, int base)
{
    int count = 0;
    while (number > 0)
    {
        number /= base;
        count++;
    }
    return count;
}

status is_new_operation(Current_settings_ptr settings, char * string, operation * operation_name)
{
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (strcmp(string, settings->operations_names[i]) == 0)
        {
            *operation_name = i;
            return success;
        }
    }
    return fail;
}

status is_operation(char * string, operation * operation_name)
{
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (strcmp(string, start_operations_names[i]) == 0)
        {
            *operation_name = i;
            return success;
        }
    }
    return fail;
}

status is_variable(char * string)
{
    if (!string) return invalid_lexeme;
    if (strlen(string) == 0) return invalid_lexeme;
    if (isdigit(string[0])) return invalid_variable;
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!isdigit(string[i]) && !isalpha(string[i]) && string[i] != '_') return invalid_variable;
    }
    return success;
}

status is_number(char * string)
{
    for (int i = 0; string[i] != 0; ++i)
    {
        if (!isdigit(string[i]) && !isalpha(string[i])) return invalid_variable;
    }
    return success;
}

status is_debug(char * string)
{
    int count = 3;
    char * variations[] = {"--debug", "-d", "/debug"};
    for (int i = 0; i < count; ++i) if (!strcmp(variations[i], string)) return success;
    return fail;
}