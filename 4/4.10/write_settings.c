#include "write_settings.h"

status write_new_settings(Current_settings_ptr settings)
{
    FILE * config_file = fopen(CONFIG_FILE, "w");
    if (!config_file) return file_open_error;

    if (settings->operation_result_type == left) fprintf(config_file, "left=\n");
    else fprintf(config_file, "right=\n");

    for (int i = 0; i < OPERATIONS_COUNT; ++i)
    {
        fprintf(config_file, "%s %s\n", start_operations_names[i], settings->operations_names[i]);
        char * syntax;
        if (settings->basic_syntax[i] == left) fprintf(config_file, "%s()\n", start_operations_names[i]);
        else if (settings->basic_syntax[i] == right) fprintf(config_file, "()%s\n", start_operations_names[i]);
        else if (i != OPERATIONS_COUNT - 1 && settings->basic_syntax[i] == middle) fprintf(config_file, "(%s)\n", start_operations_names[i]);
    }
    fclose(config_file);
    return success;
}