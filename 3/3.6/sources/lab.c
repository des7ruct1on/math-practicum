#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}

status_cmd command(char** arg_one, char** arg_two) {
    char* cmd = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!cmd) {
        return cmd_error_alloc;
    }
    int index = 0;
    char symbol = getchar();
    while (!isspace(symbol)) {
        cmd[index++] = symbol;
        symbol = getchar();
    }
    cmd[index] = '\0';
    index = 0;
    if (!strcmp(cmd, "Exit")) {
        free(cmd);
        return cmd_exit;
    }  else if (!strcmp(cmd, "Print")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        return cmd_print;
    } else if (!strcmp(cmd, "Routes")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_two)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_two)[index] = '\0';
        index = 0;
        return cmd_route;
    } else if (!strcmp(cmd, "Time")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_two)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_two)[index] = '\0';
        index = 0;
        return cmd_time;
    } else if (!strcmp(cmd, "Stop")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_two)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_two)[index] = '\0';
        index = 0;
        return cmd_stop;
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}

Vehicle* find_vehicle(char* number, Vehicle* head) {
    //printf("%s key\n", number);
    if (!number) {
        return NULL;
    }
    Vehicle* current = head;
    while(current != NULL) {
        if (!strcmp(number, current->number)) {
            //printf("not-Null\n");
            return current;
        }
        current = current->next;
    }
    //printf("Null2\n");
    return NULL;
}

status_code create_vehicle(char* number, Vehicle** new) {
    if (!number) return code_invalid_parameter;

    (*new) = malloc(sizeof(Vehicle));
    if (!(*new)) {
        return code_error_alloc;
    }
    (*new)->routes = NULL;
    (*new)->next = NULL;
    (*new)->number = number;
    //free(number);
    //number = NULL;
    //if (!(*new)->number) return code_error_alloc;

    return code_success;
}

int compare_time(char* a, char* b) {
    int l_day, l_month, l_year, l_hour, l_minute, l_sec;
    int r_day, r_month, r_year, r_hour, r_minute, r_sec;
    if (!a && !b) return 0;
    if (!a) return 1;
    if (!b) return -1;
    sscanf(a, "%d.%d.%d %d:%d:%d", &l_day, &l_month, &l_year, &l_hour, &l_minute, &l_sec);
    sscanf(b, "%d.%d.%d %d:%d:%d", &r_day, &r_month, &r_year, &r_hour, &r_minute, &r_sec);
    int result = 0; 
    result = l_year - r_year;
    if (result != 0) {
        return result;
    }
    result = l_month - r_month;
    if ((result = l_month - r_month) != 0) {
        return result;
    }
    if ((result = l_day - r_day) != 0) {
        return result;
    }
    if ((result = l_hour - r_hour) != 0) {
        return result;
    }

    if ((result = l_minute - r_minute) != 0) {
        return result;
    }
    return l_sec - r_sec;
}

status_code add_route(Route** _routes, char* _name, char* _time_arrival, char* _time_depart, status_stop _type) {
    if (!_name || !_time_arrival) return code_invalid_parameter;

    Route* new_route = malloc(sizeof(Route));
    if (!new_route) return code_error_alloc;

    new_route->name = strdup(_name);
    new_route->time_arrival = strdup(_time_arrival);
    new_route->time_depart = strdup(_time_depart);
    new_route->type = _type;
    new_route->next = NULL;

    if (!new_route->name || !new_route->time_arrival || !new_route->time_depart) {
        free(new_route->name);
        free(new_route->time_arrival);
        free(new_route->time_depart);
        free(new_route);
        return code_error_alloc;
    }

    if (*_routes == NULL || compare_time(new_route->time_arrival, (*_routes)->time_arrival) < 0) {
        new_route->next = *_routes;
        *_routes = new_route;
        return code_success;
    }

    Route* current = *_routes;
    while (current->next != NULL && compare_time(new_route->time_arrival, current->next->time_arrival) >= 0) {
        current = current->next;
    }

    new_route->next = current->next;
    current->next = new_route;

    return code_success;
}


status_stop get_status_stop(char* str) {
    if (!strcmp(str, "End")) return stop_end;
    if (!strcmp(str, "Start")) return stop_start;
    if (!strcmp(str, "Mid")) return stop_mid;
    else return stop_error;
 }

status_code add_vehicle(Vehicle** vehicles, Vehicle* add) {
    if (!add) return code_invalid_parameter;
    
    if (!(*vehicles)) {
        *vehicles = add;
    } else if (!(*vehicles)->next) {
        (*vehicles)->next = add;
    } else {
        Vehicle* current = *vehicles;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = add;
    }
    return code_success;
}

status_code read_from_file(char* argv[], int argc, Vehicle** vehicles) {
    //(*vehicles) = malloc(sizeof(Vehicle) * argc);
    //if (!(*vehicles)) {
        //return code_error_alloc;
    //}
    status_free st_free;
    status_code st_act;
    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        FILE* in = fopen(filename, "r");
        //printf("        %s\n", filename);
        if (!in) return code_error_oppening;
        char* line = NULL;
        int read;
        while ((read = getline(&line, &(size_t){0}, in)) != -1) {
            if (read == -1) {
                free(line);
                return code_error_alloc;
            }
             //printf("%d|||\n", read);
            //printf("    %s--\n", line);
            char* _number = malloc(sizeof(char) * STR_SIZE);
            if (!_number) {
                st_free = free_all(1, line);
                fclose(in);
                return code_error_alloc;
            }
            char* _name = malloc(sizeof(char) * STR_SIZE);
            if (!_name) {
                st_free = free_all(2, line, _number);
                fclose(in);
                return code_error_alloc;
            }
            char* _time_arr_date = malloc(sizeof(char) * STR_SIZE);
            if (!_time_arr_date) {
                st_free = free_all(3, line, _number, _name);
                fclose(in);
                return code_error_alloc;
            }
            char* _time_arr_time = malloc(sizeof(char) * STR_SIZE);
            if (!_time_arr_time) {
                st_free = free_all(3, line, _number, _name);
                fclose(in);
                return code_error_alloc;
            }
            char* _time_dep_date = malloc(sizeof(char) * STR_SIZE);
            if (!_time_dep_date) {
                st_free = free_all(4, line, _number, _name, _time_arr_date);
                fclose(in);
                return code_error_alloc;
            }
            char* _time_dep_time = malloc(sizeof(char) * STR_SIZE);
            if (!_time_dep_time) {
                st_free = free_all(5, line, _number, _name, _time_arr_date, _time_dep_date);
                fclose(in);
                return code_error_alloc;
            }
            char* _stop_type = malloc(sizeof(char) * STR_SIZE);
            if (!_stop_type) {
                st_free = free_all(6, line, _number, _name, _time_arr_date, _time_dep_date, _time_dep_time);
                fclose(in);
                return code_error_alloc;
            }
            if (sscanf(line, "%s %s %s %s %s %s %s", _number, _name, _time_arr_date, _time_arr_time, _time_dep_date, _time_dep_time, _stop_type) != 7) {
                st_free = free_all(7, line, _number, _name, _time_arr_date, _time_dep_date, _time_dep_time, _stop_type);
                fclose(in);
                return code_invalid_parameter;
            }
            
            Vehicle* new = NULL;
            new = find_vehicle(_number, *vehicles);
            bool is_first = true;
            if (!new) {
                is_first = true;
            } else {
                is_first = false;
            }
            strcat(_time_arr_date, " ");
            strcat(_time_arr_date, _time_arr_time);
            free(_time_arr_time);
            _time_arr_time = NULL;
            strcat(_time_dep_date, " ");
            strcat(_time_dep_date, _time_dep_time);
            free(_time_dep_time);
            _time_dep_time = NULL;
            //printf("        {%s %s %s %s %s---\n", _number, _name, _time_arr_date, _time_dep_date, _stop_type);
            
            if (is_first) {
                st_act = create_vehicle(_number, &new);
                if (st_act == code_error_alloc) {
                    st_free = free_all(6, _stop_type, line, _number, _name, _time_arr_date, _time_dep_date);
                    fclose(in);
                    return code_error_alloc; 
                }
            }
            status_stop st_stop;
            st_stop = get_status_stop(_stop_type);
            if (st_stop == stop_error) {
                st_free = free_all(6, _stop_type, line, _number, _name, _time_arr_date, _time_dep_date);
                fclose(in);
                return code_invalid_parameter; 
            }
            st_act = add_route(&new->routes, _name, _time_arr_date, _time_dep_date, st_stop);
            if (st_act != code_success) {
                st_free = free_all(6, _stop_type, line, _number, _name, _time_arr_date, _time_dep_date);
                fclose(in);
                return st_act; 
            }
            Route* check = new->routes;
            //printf("%s %s %s\n", check->name, check->time_arrival, check->time_depart);
            if (is_first || !new) {
                st_act = add_vehicle(vehicles, new);
                if (st_act != code_success) {
                    st_free = free_all(6, _stop_type, line, _number, _name, _time_arr_date, _time_arr_date);
                    fclose(in);
                    return st_act; 
                }
            }
            //st_free = free_all(6, _stop_type, line, _number, _name, _time_arr_date, _time_dep_date);
            st_free = free_all(1, _stop_type);
            free(line);
            line = NULL;
        }
        if (line) free(line);
        //printf("!!!!\n");
        fclose(in);
    }
    return code_success;
}

status_code print_transports(Vehicle* vehicles, char* filename) {
    if (!vehicles) return code_error_alloc;
    if (!filename) return code_invalid_parameter;
    Vehicle* current = vehicles;
    FILE* out = fopen(filename, "a+");
    if (!out) return code_error_oppening;
    while (current != NULL) {
        int index_route = 0;
        fprintf(out, "Number: %s", current->number);
        Route* _routes = current->routes;
        printf("<<<%d\n", size_list(_routes));
        while (_routes != NULL) {
            if (_routes->type == stop_start) {
                fprintf(out, "\nRoute %d: ", index_route);
                index_route++;
            } else {
                fprintf(out, "-> ");
            }
            fprintf(out, "%s ", _routes->name);
            _routes = _routes->next;
        }
        current = current->next;
        fprintf(out, "\n");
    }
    fclose(out);
    return code_success;
}

status_code print_transport_info(Vehicle* vehicle, char* filename) {
    if (!vehicle) return code_error_alloc;
    if (!filename) return code_invalid_parameter;
    FILE* out = fopen(filename, "a+");
    if (!out) return code_error_oppening;
    int index_route = 0;
    fprintf(out, "Number: %s", vehicle->number);
    Route* _routes = vehicle->routes;
    while (_routes != NULL) {
        if (_routes->type == stop_start) {
            fprintf(out, "\nRoute %d: ", index_route);
            index_route++;
        } else {
            fprintf(out, "-> ");
        }
        fprintf(out, "%s ", _routes->name);
        _routes = _routes->next;
    }
    fprintf(out, "\n");
    fclose(out);
    return code_success;
}

void free_routes(Route* routes) {
    if (!routes) return;

    Route* current = routes;
    int size = size_list(routes);
    for (int i = 0; i < size; i++) {
        free(current->name);
        current->name = NULL;
        free(current->time_arrival);
        current->time_arrival = NULL;
        free(current->time_depart);
        current->time_depart = NULL;
        Route* tmp = current->next;
        free(current);
        current = tmp;
    }
}

void free_transports(Vehicle* vehicles) {
    if (!vehicles) return;
    Vehicle* current = vehicles;
    while (current != NULL) {
        Vehicle* tmp = current->next;
        free_routes(current->routes);
        free(current);
        current = tmp;
    }
}

int size_list(Route* routes) {
    int count = 0;
    if (!routes) return count;
    Route* cur = routes;
    while (cur != NULL) {
        printf("%s ", cur->name);
        count++;
        cur = cur->next;
    }
    return count;
}

int num_of_routes(Route* routes) {
    int count = 0;
    if (!routes) return count;
    Route* cur = routes;
    while (cur != NULL) {
        if (cur->type == stop_start) count++;
        cur = cur->next;
    }
    return count;
}

time_t get_time_difference_minutes(char* time1, char* time2) {
    int day_1, month_1, year_1, hours_1, minutes_1, seconds_1;
    int day_2, month_2, year_2, hours_2, minutes_2, seconds_2;

    sscanf(time1, "%d.%d.%d %d:%d:%d", &day_1, &month_1, &year_1, &hours_1, &minutes_1, &seconds_1);
    sscanf(time2, "%d.%d.%d %d:%d:%d", &day_2, &month_2, &year_2, &hours_2, &minutes_2, &seconds_2);

    struct tm date1 = {0};
    struct tm date2 = {0};

    date1.tm_year = year_1 - 1900;
    date1.tm_mon = month_1 - 1;
    date1.tm_mday = day_1;
    date1.tm_hour = hours_1;
    date1.tm_min = minutes_1;
    date1.tm_sec = seconds_1;

    date2.tm_year = year_2 - 1900;
    date2.tm_mon = month_2 - 1;
    date2.tm_mday = day_2;
    date2.tm_hour = hours_2;
    date2.tm_min = minutes_2;
    date2.tm_sec = seconds_2;

    time_t time1_seconds = mktime(&date1);
    time_t time2_seconds = mktime(&date2);

    time_t diff = difftime(time2_seconds, time1_seconds);
    printf("%ld---\n", diff);
    return diff;
}



Vehicle* find_least_routes_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* least_vehicle = vehicles;
    int least = num_of_routes(current->routes);
    current = current->next;
    int tmp = 0;
    while (current != NULL) {
        tmp = num_of_routes(current->routes);
        if (tmp <= least) {
            least = tmp;
            least_vehicle = current;
        }
        current = current->next;
    }
    return least_vehicle;
}

Vehicle* find_biggest_routes_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* max_vehicle = NULL;
    int max = 0;
    int tmp = 0;
    while (current != NULL) {
        tmp = num_of_routes(current->routes);
        if (tmp > max) {
            max = tmp;
            max_vehicle = current;
        }
        current = current->next;
    }
    return max_vehicle;
}

Vehicle* find_longest_routes_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* max_vehicle = NULL;
    long long max = 0;
    long long tmp = 0;
    while (current != NULL) {
        Route* stop = current->routes->next;
        while (stop->type != stop_end) {
            stop = stop->next;
        }
        tmp = get_time_difference_minutes(current->routes->time_arrival, stop->time_depart);
        if (tmp > max) {
            max = tmp;
            max_vehicle = current;
        }
        current = current->next;
    }
    return max_vehicle;
}

Vehicle* find_fastest_routes_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* least_vehicle = NULL;
    long long least = LLONG_MAX;
    long long tmp = 0;
    bool start = true;
    while (current != NULL) {
        Route* stop = current->routes->next;
        while (stop->type != stop_end) {
            stop = stop->next;
        }
        tmp = get_time_difference_minutes(current->routes->time_arrival, stop->time_depart);
        if (tmp < least) {
            least = tmp;
            least_vehicle = current;
        }
        current = current->next;
    }
    return least_vehicle;
}

Vehicle* find_fastest_stop_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* least_vehicle = NULL;
    long long least = LLONG_MAX;
    long long tmp = 0;
    bool start = true;
    if (vehicles->next == NULL) {
        return vehicles;
    }
    while (current->next != NULL) {
        tmp = get_time_difference_minutes(current->routes->time_depart, current->next->routes->time_arrival);
        if (tmp < least) {
            least = tmp;
            least_vehicle = current;
        }
        current = current->next;
    }
    return least_vehicle;
}

Vehicle* find_longest_stop_vehicle(Vehicle* vehicles) {
    if (!vehicles) return NULL;
    Vehicle* current = vehicles;
    Vehicle* max_vehicle = NULL;
    long long max = 0;
    long long tmp = 0;
    bool start = true;
    if (vehicles->next == NULL) {
        return vehicles;
    }
    while (current->next != NULL) {
        tmp = get_time_difference_minutes(current->routes->time_depart, current->next->routes->time_arrival);
        if (tmp > max) {
            max = tmp;
            max_vehicle = current;
        }
        current = current->next;
    }
    return max_vehicle;
}
