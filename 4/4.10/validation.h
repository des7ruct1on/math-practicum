#ifndef _VALIDATION_H
#define _VALIDATION_H

#include "errors.h"
#include <ctype.h>
#include "types.h"
#include <string.h>

status check_if_in_base(char * number, int base);
int get_symbols_count(uint32_t number, int base);
status is_variable(char * string);
status is_operation(char * string, operation * operation_name);
status is_number(char * string);
status is_new_operation(Current_settings_ptr settings, char * string, operation * operation_name);
status is_debug(char * string);

#endif