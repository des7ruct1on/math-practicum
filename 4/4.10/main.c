#include "read_file.h"
#include "write_settings.h"

int main(int argc, char *argv[])
{
    if (argc > 7 || argc < 2)
    {
        print_error(command_line_arguments_error);
        return command_line_arguments_error;
    }

    int debug = 0;
    if (is_debug(argv[argc - 1]) == success) debug = 1;

    // сначала считываем сохраненные настройки
    status error;
    Current_settings_ptr settings = create_current_settings(debug);
    // todo
    if (settings) // если там что-то есть
    {
        FILE * start_settings = fopen(CONFIG_FILE, "r");
        if (!start_settings)
        {
            free_current_settings(settings);
            print_error(file_open_error);
            return file_open_error;
        }
        if ((error = read_file_with_settings(start_settings, settings)) != success)
        {
            fclose(start_settings);
            free_current_settings(settings);
            print_error(error);
            return error;
        }
        fclose(start_settings);
    }

    int assign_base = 10;
    int input_base = 10;
    int output_base = 10;

    // проверяем, есть ли bases :)
    if (argc >= 3)
    {
        if (sscanf(argv[2], "%d\n", &assign_base) < 0 || (assign_base < 2 || assign_base > 36)) 
        {
            free_current_settings(settings);
            print_error(invalid_base);
            return error;
        }
        if (argc >= 4)
        {
            if (sscanf(argv[3], "%d\n", &input_base) < 0 || (input_base < 2 || input_base > 36)) 
            {
                free_current_settings(settings);
                print_error(invalid_base);
                return error;
            }
        }
        if (argc >= 5)
        {
            if (sscanf(argv[4], "%d\n", &output_base) < 0 || (output_base < 2 || output_base > 36)) 
            {
                free_current_settings(settings);
                print_error(invalid_base);
                return error;
            }
        }
    }

    // если пользователь кинул файл с новыми настройками, добавляем их
    if (argc >= 6)
    {
        FILE * new_settings = fopen(argv[5], "r");
        if (!new_settings)
        {
            free_current_settings(settings);
            print_error(file_open_error);
            return file_open_error;
        }
        if ((error = read_file_with_settings(new_settings, settings)) != success)
        {
            fclose(new_settings);
            free_current_settings(settings);
            print_error(error);
            return error;
        }
        fclose(new_settings);
        // и обновляем базовые настройки
        if ((error = write_new_settings(settings)) != success)
        {
            free_current_settings(settings);
            print_error(error);
            return error;
        }
    }

    // подключаем файлик с программой
    FILE * program = fopen(argv[1], "r");
    if (!program)
    {
        free_current_settings(settings);
        print_error(file_open_error);
        return file_open_error;
    }
    if ((error = interpretate(program, settings, input_base, assign_base, output_base)) != success)
    {
        print_error(error);
        return error;
    }
    printf("Program has finished successfuly\n");
    return success;
}