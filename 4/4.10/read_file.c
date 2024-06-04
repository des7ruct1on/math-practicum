#include "read_file.h"

status find_comment_end(char ** string)
{
    int i = 0;
    while ((*string)[i])
    {
        if ((*string)[i] == ']')
        {
            shift_string(string, i + 1);
            return success;
        }
        ++i;
    }
    if ((*string)[0] == ' ') free_from_delims(string);
    return fail;
}

status read_file_with_settings(FILE *file, Current_settings_ptr settings)
{
    if(!file) return invalid_function_argument;

    char *line = NULL;
    size_t size = 0;
    int flag = 0;
    status error;
    int command_flag = 0;
    operation operation_name;
    int operation_changing = 0;

    while(getline(&line, &size, file) > 0)
    {
        char * token;
        if ((error = my_strtok(&token, &line, " \n\t")))
        {
            free(line);
            return error;
        }
        while (token)
        {
            if (operation_changing)
            {
                change_name(settings, operation_name, token);
                command_flag = 1;
                operation_changing = 0;
                free(token);
                token = NULL;
                if ((error = my_strtok(&token, &line, " \n\t")))
                {
                    free(line);
                    return error;
                }
                continue;
            }
            if (flag && find_comment_end(&token) == success) flag = 0;
            if (!token[0])
            {
                free(token);
                if ((error = my_strtok(&token, &line, " \n\t")))
                {
                    free(line);
                    return error;
                }
                continue;
            }
            if (token[0] == '#') break;

            else if (token[0] == '[') flag = 1;
            if (flag && find_comment_end(&token) == success) flag = 0;
            else if (flag)
            {
                free(token);
                token = NULL;
                if ((error = my_strtok(&token, &line, " \n\t")))
                {
                    free(line);
                    return error;
                }
                continue;
            }

            size_t token_size = strlen(token);

            if (command_flag && !flag)
            {
                free(token);
                free(line);
                return invalid_settings_file;
            }
            if (!strcmp(token, "right=") && !flag)
            {
                settings->operation_result_type = right;
                command_flag = 1;
            }
            else if (!strcmp(token, "left=") && !flag)
            {
                settings->operation_result_type = left;
                command_flag = 1;
            }
            else if (token[0] == '(' && token[1] == ')' && !flag)
            {
                if (is_operation(token + 2, &operation_name) == success) change_basic_syntax(settings, operation_name, right);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            else if (token[token_size - 1] == ')' && token_size - 2 >= 0 && token[token_size - 2] == '(' && !flag)
            {
                token[token_size - 1] = token[token_size - 2] = 0;
                if (is_operation(token, &operation_name) == success) change_basic_syntax(settings, operation_name, left);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            else if (token[0] == '(' && token_size - 1 >= 0 && token[token_size - 1] == ')' && !flag)
            {
                token[token_size - 1] = 0;
                if (is_operation(token + 1, &operation_name) == success && settings->basic_types[operation_name] == binary) change_basic_syntax(settings, operation_name, middle);
                else
                {
                    free(token);
                    free(line);
                    return invalid_settings_file;
                }
                command_flag = 1;
            }
            else if (is_operation(token, &operation_name) == success) operation_changing = 1;
            else
            {
                free(token);
                free(line);
                return invalid_settings_file;
            }

            free(token);

            if ((error = my_strtok(&token, &line, " \n\t")))
            {
                free(line);
                return error;
            }
        }
        command_flag = 0;
        if (token) free(token);
        free(line);
        line = NULL;
        size = 0;
        if (operation_changing) return invalid_settings_file;
    }
    free(line);
    line = NULL;
    return success;
}