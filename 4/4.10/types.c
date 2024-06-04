#include "types.h"

#include <stdlib.h>

Current_settings_ptr create_current_settings(int debug)
{
    operation_syntax start_basic_syntax[] = {left, left, left, left, left, left, left, left, left, left, left, left};
    operation_type start_basic_types[] = {unary, unary, unary, binary, binary, binary, binary, binary, binary, binary, binary, binary, binary};
    operation_syntax start_operation_result_type = left;

    Current_settings_ptr settings = (Current_settings_ptr)malloc(sizeof(Current_settings));
    if (!settings) return NULL;
    settings->operations_names = (char**)(calloc(OPERATIONS_COUNT, sizeof(char*)));
    if (!settings->operations_names)
    {
        free(settings);
        return NULL;
    }
    settings->basic_syntax = (operation_syntax *)calloc(OPERATIONS_COUNT, sizeof(operation_syntax));
    if (!settings->basic_syntax)
    {
        free(settings->operations_names);
        free(settings);
        return NULL;
    }
    settings->basic_types = (operation_type *)calloc(OPERATIONS_COUNT, sizeof(operation_syntax));
    if (!settings->basic_types)
    {
        free(settings->operations_names);
        free(settings->basic_syntax);
        free(settings);
        return NULL;
    }
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        char * tmp = (char*)calloc(strlen(start_operations_names[i]) + 1, sizeof(char));
        if (!tmp)
        {
            for (int j = 0; j < i; ++j) free(settings->operations_names[j]);
            free(settings->operations_names);
            free(settings->basic_syntax);
            free(settings);
            return NULL;
        }
        strcpy(tmp, start_operations_names[i]);
        settings->operations_names[i] = tmp;
        settings->basic_syntax[i] = start_basic_syntax[i];
        settings->basic_types[i] = start_basic_types[i];
    }
    settings->operation_result_type = start_operation_result_type;
    settings->debug = debug;
    return settings;
}

void free_current_settings(Current_settings_ptr settings)
{
    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        if (settings->operations_names[i]) free(settings->operations_names[i]);
    }
    free(settings->operations_names);
    free(settings->basic_syntax);
    free(settings->basic_types);
    free(settings);
    settings = NULL;
}

void change_name(Current_settings_ptr settings, operation operation_id, char * new_name)
{
    char * tmp = (char*)calloc(strlen(new_name) + 1, sizeof(char));
    strcpy(tmp, new_name);
    if (settings->operations_names[operation_id]) free(settings->operations_names[operation_id]);
    settings->operations_names[operation_id] = tmp;
}

void change_basic_syntax(Current_settings_ptr settings, operation operation_id, operation_syntax new_syntax)
{
    settings->basic_syntax[operation_id] = new_syntax;
}

void change_operation_result_type(Current_settings_ptr settings, operation_syntax new_res_type)
{
    settings->operation_result_type = new_res_type;
}