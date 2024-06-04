#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

typedef enum
{
    success,
    command_line_arguments_error,
    no_memory,
    fail,
    file_open_error,
    invalid_function_argument,
    invalid_base,
    invalid_variable,
    not_in_base,
    cannot_find,
    invalid_lexeme,
    invalid_settings_file

} status;

void print_error(status const error);

status free_all_strings(status error, int count, ...);

#endif