#include "interpreter.h"


status add_to_expression(char ** expression, char * to_add)
{
    size_t old_size = strlen(*expression);
    size_t new_size = old_size + strlen(to_add);
    char * tmp = realloc(*expression, (new_size + 2) * sizeof(char));
    if (!tmp)
    {
        free(expression);
        return no_memory;
    }
    *expression = tmp;
    if (old_size != 0)
    {
        (*expression)[old_size] = ',';
        old_size++;
    }
    strcpy(*expression + old_size, to_add);
    return success;
}

status make_unary_expression(Trie_ptr trie, char * variable_name, operation operation_name, uint32_t value, uint32_t * result, int input_base, int output_base, int assign_base)
{
    switch (operation_name)
    {
        case NOT:
            return not_operation(trie, value, result);
        case INPUT:
            return input_operation(trie, input_base, result, variable_name);
        case OUTPUT:
            return output_operation(trie, value, output_base);
        default:
            break;
    }
    return fail;
}

status make_binary_expression(Trie_ptr trie, operation operation_name, uint32_t value_1, uint32_t value_2, uint32_t * result)
{
    switch (operation_name)
    {
        case ADD:
            return add_operation(trie, value_1, value_2, result);
        case MULT:
            return mult_operation(trie, value_1, value_2, result);
        case SUB:
            return sub_operation(trie, value_1, value_2, result);
        case POW:
            return pow_operation(trie, value_1, value_2, result);
        case DIV:
            return div_operation(trie, value_1, value_2, result);
        case REM:
            return rem_operation(trie, value_1, value_2, result);
        case XOR:
            return xor_operation(trie, value_1, value_2, result);
        case AND:
            return and_operation(trie, value_1, value_2, result);
        case OR:
            return or_operation(trie, value_1, value_2, result);
        default:
            break;
    }
    return fail;
}

status is_delim(char character, const char * delim)
{
    int i = 0;
    while (delim[i])
    {
        if (delim[i] == character) return success;
        ++i;
    }
    return fail;
}

void shift_string(char ** string, int index)
{
    int i = 0;
    int start_index = index;
    while ((*string)[index])
    {
        (*string)[i] = (*string)[index];
        ++i;
        ++index;
    }
    for (int j = i; j < index; ++j) (*string)[j] = 0;
}

status my_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return success;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return no_memory;
    
    int i = 0;

    while (string[i] && is_delim(string[i], delim) != success)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (i == 0 && is_delim(string[i], delim) == success) 
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return success;
    }
    if (strcmp(delim, ";") == 0 && is_delim(string[i], delim) != success)
    {
        free(new_string);
        printf("tut\n");
        return invalid_lexeme;
    }
    if (is_delim(string[i], delim) == success) ++i;

    shift_string(st_string, i);
    if (strcmp(delim, ";") != 0) free_from_delims(&new_string);
    *result = new_string;
    return success;
}

// тк "=" - тоже функция, пользователь может заменить то, как она выглядит, на многосимвольное выражение. поэтому для него сделаем отдельный strtok
status my_assign_strtok(char ** result, char ** st_string, const char * delim)
{
    int is_empty_string = 1;
    char * string = *st_string;
    *result = NULL;
    if (!string[0]) return success;
    char * new_string = (char*)calloc((strlen(string) + 1), sizeof(char));
    if (!new_string) return no_memory;
    int i = 0;

    char * ptr = strstr(string, delim);
    if (!ptr)
    {
        strcpy(new_string, *st_string);
        shift_string(st_string, strlen(string));
        free_from_delims(&new_string);
        *result = new_string;
        return success;
    }
    while (string[i] != *ptr)
    {
        if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t') is_empty_string = 0;
        new_string[i] = string[i];
        ++i;
    }
    if (string[0] == *ptr)
    {
        shift_string(st_string, i + 1);
        free(new_string);
        return my_assign_strtok(result, st_string, delim);
    }
    if (is_empty_string)
    {
        free(new_string);
        return success;
    }
    shift_string(st_string, i + strlen(delim));
    
    free_from_delims(&new_string);
    *result = new_string;
    return success;
}

int check_nesting(char * expression)
{
    int count = 0;
    int close_count = 0;
    for (int i = 0; expression[i] != 0; ++i)
    {
        if (expression[i] == '(') count++;
        else if (expression[i] == ')') close_count++;
    }
    if (count != close_count) return -1;
    return count;
}

status solve_expression(Current_settings_ptr settings, Trie_ptr trie, char * st_expression, uint32_t * result, int input_base, int output_base, int assign_base)
{
    char * expression = NULL;
    char * token = NULL;
    char * tok = NULL;
    char * tmp_expression = NULL;
    char * to_solve_1 = NULL;
    char * to_solve_2 = NULL;
    int count = 5;
    operation operation_name;
    status error = success;
    int tmp_nesting = 0;
    int arguments_count = 0;
    int commands_count = 0;
    uint32_t value_1;
    uint32_t value_2;
    int nesting = check_nesting(st_expression);
    if (!st_expression || nesting < 0) {
        printf("tut2\n");
        return invalid_lexeme;
    }
    if (!nesting) nesting = 1;


    expression = (char*)calloc(strlen(st_expression) + 1, sizeof(char));
    if (!expression) return no_memory;
    strcpy(expression, st_expression);
    //printf("EX{RESSION}: %s\n", expression);
    if ((error = my_strtok(&token, &expression, "(), ")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
    tmp_expression = (char*)calloc(1, sizeof(char));
    if (!tmp_expression)
    {
        error = no_memory;
        return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
    }
    tmp_nesting = nesting;
    
    while (token)
    {   
        //printf("token: %s\n", token);
        if (token[0] && is_new_operation(settings, token, &operation_name) == success)
        {
            commands_count++;
            if ((settings->basic_syntax[operation_name] == left && arguments_count / commands_count > 1) ||
                (settings->basic_syntax[operation_name] == middle && 
                (settings->basic_types[operation_name] == unary || arguments_count / commands_count != 1)) ||
                (settings->basic_syntax[operation_name] == right && 
                ((settings->basic_types[operation_name] == binary && arguments_count / commands_count != 2) ||
                (settings->basic_types[operation_name] == unary && arguments_count / commands_count != 1)))
                )
            {
                error = invalid_lexeme;
                //printf("tut3\n");
                return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            tmp_nesting--;
            if (tmp_nesting <= 0) break;
        }
        if (token[0] && (error = add_to_expression(&tmp_expression, token)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
        if (token[0]) arguments_count++;
        //printf("TMP expression: %s\n", tmp_expression);
        free(token);
        token = NULL;

        if ((error = my_strtok(&token, &expression, "(), ")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
    }
    if (!token)
    {
        if (is_variable(tmp_expression) == success || is_number(tmp_expression) == success)
        {
            error = get_value(trie, tmp_expression, result, assign_base);
            return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
        }
        error = invalid_lexeme;
        //printf("%s---\n", tmp_expression);
        //printf("tut4\n");
        return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
    }
    // если токен - команда
    if (settings->basic_syntax[operation_name] != left && nesting && commands_count < nesting)
    {
        error = invalid_lexeme;
        //printf("tut5\n");
        return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
    }

    free(token);
    token = NULL;

    switch (settings->basic_types[operation_name])
    {
        case unary:
            switch (settings->basic_syntax[operation_name])
            {
                case left:
                    if((error = my_strtok(&to_solve_1, &expression, "(),")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
                    break;

                case right:
                    to_solve_1 = tmp_expression;
                    break;

                default:
                    error = invalid_lexeme;
                    printf("tut6\n");
                    return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            if ((error = solve_expression(settings, trie, to_solve_1, &value_1, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);;
            if ((error = make_unary_expression(trie, to_solve_1, operation_name, value_1, result, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);;
            break;

        case binary:
            to_solve_1 = (char*)calloc(1, sizeof(char));
            if (!to_solve_1)
            {
                error = no_memory;
                return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            to_solve_2 = (char*)calloc(1, sizeof(char));
            if (!to_solve_2)
            {
                error = no_memory;
                return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            char * tmp_expr;
            // первый аргумент
            switch (settings->basic_syntax[operation_name])
            {
                case left:
                    tmp_nesting = nesting;
                    tmp_expr = expression;
                    break;

                case right:
                    tmp_nesting = nesting;
                    tmp_expr = tmp_expression;
                    break;

                case middle:
                    tmp_nesting = nesting + 1;
                    tmp_expr = tmp_expression;
                    break;
            }
            if ((error = my_strtok(&tok, &tmp_expr, "(),")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            while (tok && tmp_nesting > 0)
            {
                if ((error = add_to_expression(&to_solve_1, tok)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
                free(tok);
                tok = NULL;
                tmp_nesting--;
                if (tmp_nesting <= 0) break;
                if ((error = my_strtok(&tok, &tmp_expr, "(),")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            if (tok) free(tok);

            // второй аргумент
            switch (settings->basic_syntax[operation_name])
            {
                case left:
                    tmp_nesting = nesting;
                    tmp_expr = expression;
                    break;

                case right:
                    tmp_nesting = nesting;
                    tmp_expr = tmp_expression;
                    break;

                case middle:
                    tmp_expr = expression;
                    tmp_nesting = nesting;
                    break;
            }

            if ((error = my_strtok(&tok, &tmp_expr, "(),")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            while (tok && tmp_nesting > 0)
            {
                if ((error = add_to_expression(&to_solve_2, tok)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
                free(tok);
                tok = NULL;
                tmp_nesting--;
                if (tmp_nesting <= 0) break;
                if ((error = my_strtok(&tok, &tmp_expr, "(),")) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            }
            //printf("%d -- %d\n", value_1, value_2);
            if ((error = solve_expression(settings, trie, to_solve_1, &value_1, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
            if ((error = solve_expression(settings, trie, to_solve_2, &value_2, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);;
            if ((error = make_binary_expression(trie, operation_name, value_1, value_2, result)) != success) return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);;
            break;
    }
    
    return free_all_strings(error, count, expression, token, tmp_expression, to_solve_1, to_solve_2);
}

status free_from_delims(char ** string)
{
    if (!*string) return success;
    int i = 0;
    while ((*string)[i] && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t')) ++i;
    shift_string(string, i);
    i = strlen(*string) - 1;
    while (i != 0 && ((*string)[i] == ' ' || (*string)[i] == '\n' || (*string)[i] == '\t'))
    {
        (*string)[i] = 0;
        --i;
    }
    return success;
}

status check_comments(char ** st_string, int * is_breakpoint)
{
    char * string = *st_string;
    int i = 0;
    while (string[i] && string[i] != '#') ++i;
    if (!string[i]) return fail;
    int j = i;
    while (string[j] && string[j] != '\n') ++j;
    if (!string[j]) return success; // комментарий еще продолжается
    // если же нашли конец, находится ли внутри BREAKPOINT или нужно просто удалить комментарий (удаляем, просто заменяя все на пробелы)

    // проверка, лежит ли внутри брейкпоинт
    int no = 0;
    char * breakpoint = "BREAKPOINT";
    if (strlen(breakpoint) != j - (i + 1)) no = 1;
    if (no)
    {
        for (int h = i + 1; h < j; ++h)
        {
            if (string[h] != breakpoint[h])
            {
                no = 1;
                break;
            }
        }
    }
    if (!no) *is_breakpoint = 1;
    // теперь просто убираем комментарий
    for (int h = i; h < j; ++h) (*st_string)[h] = ' ';
    return fail;
}

status scan_buffer(Current_settings_ptr settings, Trie_ptr trie, char * st_buffer, int input_base, int output_base, int assign_base)
{
    char * buffer = NULL;
    status error = success;
    char * line = NULL;
    int comment = 0;
    int i = 0;
    int breakpoint = 0;
    char * line_copy_1 = NULL;
    char * line_copy = NULL;
    char * string = NULL;
    char * copy_res = NULL;
    uint32_t value;
    char * variable_name = NULL;
    char * string_copy = NULL;
    char * expression = NULL;
    int count = 9;

    buffer = (char*)calloc(strlen(st_buffer) + 1, sizeof(char));
    if (!buffer) return no_memory;
    strcpy(buffer, st_buffer);
    if ((error = my_strtok(&line, &buffer, ";")) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);

    while (line)
    {
        if (comment)
        {
            comment = 0;
            i = 0;
            while (line[i] && line[i] != '\n')
            {
                line[i] = ' ';
                ++i;
            }
            if (!line[i])
            {
                free(line);
                if ((error = my_strtok(&line, &buffer, ";")) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                continue;
            }
        }
        free_from_delims(&line);
        if ((error = check_comments(&line, &breakpoint)) == success) // то есть комментарий не закончился
        {
            comment = 1;
            if (line[0] != '#')
            {
                line_copy_1 = (char*)malloc((strlen(line) + 1) * sizeof(char));
                if (!line_copy_1) 
                {
                    error = no_memory;
                    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(line_copy_1, line);
                if (error = my_strtok(&line, &line_copy_1, "#") != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                free(line_copy_1);
            }
            else
            {
                free(line);
                line = NULL;
                if (error = my_strtok(&line, &buffer, ";") != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                continue;
            }
        }
        if (breakpoint && settings->debug) 
        {
            int flag = 1;
            if ((error = debugger(trie, &flag)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            if (!flag) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            breakpoint = 0;
        }
        line_copy = (char*)malloc((strlen(line) + 1) * sizeof(char));
        if (!line_copy)
        {
            error = no_memory;
            return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        }
        strcpy(line_copy, line);
        if ((error = my_assign_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        string_copy = (char*)calloc(strlen(string) + 1, sizeof(char));
        if (!string_copy)
        {
            error = no_memory;
            return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        }
        strcpy(string_copy, string);
        if ((error = my_strtok(&copy_res, &string_copy, "()")) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        if (strcmp(copy_res, settings->operations_names[2]) == success || strcmp(copy_res, settings->operations_names[1]) == success)
        {
            free(copy_res);
            copy_res = NULL;
            free(string_copy);
            string_copy = NULL;
            if ((error = solve_expression(settings, trie, string, &value, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        }
        else
        {
            free(copy_res);
            copy_res = NULL;
            if (string_copy) free(string_copy);
            string_copy = NULL;
            if (copy_res) free(copy_res);
            copy_res = NULL;
            if (settings->operation_result_type == left)
            {
                if (is_variable(string) != success)
                {
                    error = invalid_variable;
                    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!variable_name)
                {
                    error = no_memory;
                    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(variable_name, string);
                if (string) free(string);
                string = NULL;    
                if ((error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if ((error = solve_expression(settings, trie, string, &value, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
            }
            else
            {
                expression = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!expression)
                {
                    error = no_memory;
                    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(expression, string);
                if (string) free(string);
                string = NULL;
                if ((error = my_strtok(&string, &line_copy, settings->operations_names[OPERATIONS_COUNT - 1])) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if ((error = is_variable(string)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                variable_name = (char*)malloc((strlen(string) + 1) * sizeof(char));
                if (!variable_name)
                {
                    error = no_memory;
                    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                }
                strcpy(variable_name, string);
                if (string) free(string);
                string = NULL;
                if ((error = solve_expression(settings, trie, expression, &value, input_base, output_base, assign_base)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
                if (expression) free(expression);
            }
            if ((error = Trie_insert(trie, variable_name, value)) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
        }
        if (variable_name && variable_name != string) free(variable_name);
        variable_name = NULL;
        if (string) free(string);
        string = NULL;
        if (line_copy) free(line_copy);
        line_copy = NULL;
        if (line) free(line);
        line = NULL;
        if ((error = my_strtok(&line, &buffer, ";")) != success) return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
    }

    return free_all_strings(error, count, buffer, line, line_copy_1, line_copy, string, copy_res, variable_name, string_copy, expression);
}

status read_full_file(FILE * file, char ** string)
{
    size_t size = 0, capacity = 16;
    *string = (char*)calloc(capacity, sizeof(char));
    if (!*string)
    {
        fclose(file);
        return no_memory;
    }
    char c = getc(file);
    while (c != EOF)
    {
        if (size + 1 == capacity)
        {
            capacity *= 2;
            char * tmp = (char*)realloc(*string, capacity * sizeof(char));
            if (!tmp)
            {
                free(*string);
                fclose(file);
                return no_memory;
            }
            *string = tmp;
        }
        (*string)[size] = c;
        ++size;
        c = getc(file);
    }
    (*string)[size] = 0;
    fclose(file);
    return success;
}

status interpretate(FILE * file, Current_settings_ptr settings, int input_base, int assign_base, int output_base)
{
    Trie_ptr trie = Trie_create();
    if (!trie)
    {
        fclose(file);
        free_current_settings(settings);
        return file_open_error;
    }

    char * buffer = NULL;
    status error;
    if ((error = read_full_file(file, &buffer)) != success)
    {
        free_current_settings(settings);
        Trie_free(trie);
        return error;
    }

    if ((error = scan_buffer(settings, trie, buffer, input_base, output_base, assign_base)) != success)
    {
        Trie_free(trie);
        free_current_settings(settings);
        free(buffer);
        return error;
    }


    // char * prefix = (char *)calloc(1, sizeof(char));
    // if (!prefix)
    // {
    //     Trie_free(trie);
    //     free_current_settings(settings);
    //     free(buffer);
    //     return no_memory;
    // }
    // if ((error = print_all_variables(trie->root, prefix)) != success)
    // {
    //     Trie_free(trie);
    //     free_current_settings(settings);
    //     free(buffer);
    //     return error;
    // }
    // free(prefix);


    Trie_free(trie);
    free_current_settings(settings);
    free(buffer);
    return success;
}