#include "debug.h"
#include <stdio.h>

status print_all_variables(Trie_node_ptr node, char * prefix)
{
    if (!node) return success;
    size_t size = strlen(prefix);
    char * new_prefix = NULL;
    status error;

    if (node->is_final) printf("%s : %d\n", prefix, node->value);

    for (int i = 0; i < VARIABLE_CHARAS_COUNT; ++i)
    {
        new_prefix = (char*)calloc(size + 2, sizeof(char));
        if (!new_prefix) return no_memory;
        strcpy(new_prefix, prefix);

        char chara;
        if (i < 10) chara = i + '0';
        else if (i < 62) chara = i + 'A';
        else chara = '_';

        new_prefix[size] = chara;
        new_prefix[size + 1] = 0;

        if ((error = print_all_variables(node->childs[i], new_prefix)) != success)
        {
            free(new_prefix);
            return error;
        }
        free(new_prefix);
    }
    return success;
}

status print_variables(Trie_ptr trie)
{
    char * prefix = (char*)calloc(1, sizeof(char));
    if (!prefix) return no_memory;
    status error = success;
    if ((error = print_all_variables(trie->root, prefix)) != success) return free_all_strings(error, 1, prefix); 

    return free_all_strings(error, 1, prefix);
}

void print_memory_dump(void * ptr, size_t size)
{
    unsigned char * byte_ptr = (unsigned char *)ptr;
    for (size_t i = 0; i < size; ++i)
    {
        for (int j = 7; j >= 0; --j) printf("%d", (byte_ptr[i] >> j) & 1);
        if (i < size - 1) printf(" ");
    }
    printf("\n");
}

status print_variable_and_value(Trie_ptr trie)
{
    char * line = NULL;
    size_t size = 0;
    char * result = NULL;
    status error = success;
    printf("\nWrite down the variable name >> ");
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    while (is_variable(line) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    Trie_node_ptr node = NULL;
    if ((error = Trie_find(trie, line, &node)) != success) return free_all_strings(error, 2, line, result);

    if ((error = convert_to_base(node->value, 16, &result)) != success) return free_all_strings(error, 2, line, result);

    printf("variable: %s\nvalue in 16 base: %s\ndump: ", line, result);
    print_memory_dump(&(node->value), sizeof(node->value));
    printf("\n");
    return free_all_strings(error, 2, line, result);
}

status change_variable_value(Trie_ptr trie)
{
    char * line = NULL;
    size_t size = 0;
    status error = success;
    printf("\nWrite down the variable name >> ");
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    while (is_variable(line) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    char * name = line;
    // проверить переменная ли

    line = NULL;
    size = 0;
    printf("\nWrite down new value >> ");

    if (getline(&line, &size, stdin) < 0) return no_memory;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    uint32_t value;

    while ((error = get_value(trie, line, &value, 16)) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        free(line);
        line = NULL;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    Trie_node_ptr node = NULL;
    if ((error = Trie_find(trie, name, &node)) != success) return free_all_strings(error, 1, line);
    node->value = value;

    printf("\nDone!\n");
    return free_all_strings(error, 1, line);
}

status insert_new_variable(Trie_ptr trie, char ** var_name)
{
    char * line = NULL;
    size_t size = 0;
    status error = success;
    printf("\nWrite down the variable name >> ");
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;
    while (is_variable(line) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the variable name >> ");
        }
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    *var_name = line;
    // проверить переменная ли
    char * variants[] = 
    {
        "Scan value as zeckendorf;",
        "Scan value as roman."
    };
    for (int i = 0; i < 2; ++i) printf("%d. %s\n", i + 1, variants[i]);
    int res;
    printf("\nWrite down the number of function >> ");
    scanf("%d", &res);
    while (1)
    {
        if (res == 1 || res == 2) break;
        else
        {
            printf("\nTry again\n");
            while (getchar() != '\n');
        }
        printf("\nWrite down the number of function >> ");
        scanf("%d", &res);
    }
    uint32_t result;
    switch (res)
    {
        case 1:
            if ((error = insert_new_variable_zekendorf(trie, line)) != success) return error;
            break;
        case 2:
            if ((error = insert_new_variable_rim(trie, line)) != success) return error;
            break;
    }
    printf("\nDone!\n");
    return error;
}

status zeckendorf_to_decimal(char* zeckendorf, uint32_t * res) 
{
    if (strlen(zeckendorf) == 0) return invalid_function_argument;
    uint32_t decimal = 0;
    int length = strlen(zeckendorf);

    for (int i = 0; i < length; i++) 
    {
        if (zeckendorf[i] == '1') decimal += (uint32_t)pow(2, i);
        else if (zeckendorf[i] != '0') return invalid_function_argument;
    }
    *res = decimal;
    return success;
}

status insert_new_variable_zekendorf(Trie_ptr trie, char * name)
{
    char * line = NULL;
    size_t size = 0;
    status error = success;
    printf("\nWrite down the value in zeckendorf >> ");
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    uint32_t res;
    while ((error = zeckendorf_to_decimal(line, &res)) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the value in zeckendorf >> ");
        }
        free(line);
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    if ((error = Trie_insert(trie, name, res)) != success) return free_all_strings(error, 1, line);

    return free_all_strings(error, 1, line);
}

status roman_to_decimal(char* roman, uint32_t * res) 
{
    if (strlen(roman) == 0) return invalid_function_argument;
    uint32_t decimal = 0;
    int length = strlen(roman);

    for (int i = 0; i < length; i++) 
    {
        switch (roman[i]) 
        {
        case 'I':
            if (i + 1 < length && (roman[i + 1] == 'V' || roman[i + 1] == 'X')) decimal -= 1;
            else decimal += 1;
            break;
        case 'V':
            decimal += 5;
            break;
        case 'X':
            if (i + 1 < length && (roman[i + 1] == 'L' || roman[i + 1] == 'C')) decimal -= 10; 
            else decimal += 10;
            break;
        case 'L':
            decimal += 50;
            break;
        case 'C':
            if (i + 1 < length && (roman[i + 1] == 'D' || roman[i + 1] == 'M')) decimal -= 100; 
            else decimal += 100;
            break;
        case 'D':
            decimal += 500;
            break;
        case 'M':
            decimal += 1000;
            break;
        default:
            return invalid_function_argument;
        }
    }
    *res = decimal;
    return success;
}

status insert_new_variable_rim(Trie_ptr trie, char * name)
{
    char * line = NULL;
    size_t size = 0;
    status error = success;
    printf("\nWrite down the value in roman >> ");
    if (getline(&line, &size, stdin) < 0) return invalid_variable;
    size = strlen(line);
    if (line[size - 1] == '\n') line[size - 1] = 0;

    uint32_t res;
    while ((error = roman_to_decimal(line, &res)) != success)
    {
        if (strlen(line) != 0)
        {
            printf("\nTry again\n");
            printf("\nWrite down the value in roman >> ");
        }
        free(line);
        line = NULL;
        size = 0;
        if (getline(&line, &size, stdin) < 0) return invalid_variable;
        size = strlen(line);
        if (line[size - 1] == '\n') line[size - 1] = 0;
    }

    if ((error = Trie_insert(trie, name, res)) != success) return free_all_strings(error, 1, line);

    return free_all_strings(error, 1, line);
}

status remove_variable(Trie_ptr trie, char * variables[], size_t * size)
{
    status error = success;
    for (size_t i = 0; i < *size; ++i)
    {   
        Trie_node_ptr node = NULL;
        if ((error = Trie_find(trie, variables[i], &node)) != success) return error;
        node->is_final = 0;
        free(variables[i]);
        variables[i] = NULL;
    }
    *size = 0;
    printf("\nDone!\n");
    return success;
}

status add_to_names_array(char *** names, size_t * size, size_t * capacity, char * name)
{
    if ((*size) + 1 == *capacity)
    {
        *capacity *= 2;
        char ** tmp = (char**)realloc(*names, (*capacity) * sizeof(char*));
        if (!tmp) return no_memory;
        *names = tmp;
    }
    (*names)[*size] = name;
    (*size)++;
    return success;
}

status debugger(Trie_ptr trie, int * work_flag)
{
    char * new_var = NULL;
    *work_flag = 1;
    // TODO DIALOG
    status error = success;
    printf("\tDEBUGGER\n");
    int count = 7;
    char * functions[] = {
        "Print variable, its value and dump;",
        "Print all initialized variables;",
        "Change variable value;",
        "Insert new variable;",
        "Delete variables inserted while debugging;",
        "Stop debugging;",
        "Stop interpreter."
    };

    char * write_down = "Write down the number of function >> ";
    char * wrong = "Something went wrong!";
    uint32_t number;
    int function = -1;

    size_t size = 0;
    size_t capacity = 1;
    char ** new_var_names = (char**)calloc(1, sizeof(char*));
    if (!new_var_names) return no_memory;

    int flag = 1;

    while (flag)
    {
        for (int i = 0; i < count; ++i) printf("%d. %s\n", i + 1, functions[i]);
        printf("\n%s", write_down);
        scanf("%d", &number);
        switch (number)
        {
            case 1:
                if ((error = print_variable_and_value(trie)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 2:
                if ((error = print_variables(trie)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 3:
                if ((error = change_variable_value(trie)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 4:
                if ((error = insert_new_variable(trie, &new_var)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                if ((error = add_to_names_array(&new_var_names, &size, &capacity, new_var)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 5:
                if ((error = remove_variable(trie, new_var_names, &size)) != success)
                {
                    if (new_var_names)
                    {
                        for (int i = 0; i < size; ++i)
                        {
                            if (new_var_names[i])
                            {
                                free(new_var_names[i]);
                                new_var_names[i] = NULL;
                            }
                        }
                        free(new_var_names);
                        new_var_names = NULL;
                    }
                    return error;
                }
                break;
            case 6:
                flag = 0;
                break;
            case 7:
                flag = 0;
                *work_flag = 0;
                break;
            default:
                printf("\n%s\n\n", wrong);
                while (getchar() != '\n');
                break;
        }
    }
    if (new_var_names)
    {
        for (int i = 0; i < size; ++i)
        {
            if (new_var_names)
            {
                free(new_var_names[i]);
                new_var_names[i] = NULL;
            }
        }
        free(new_var_names);
        new_var_names = NULL;
    }
    return error;
}