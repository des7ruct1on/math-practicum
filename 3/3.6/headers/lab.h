#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define STR_SIZE 256

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_route,
    cmd_time,
    cmd_stop,
    cmd_print
} status_cmd;

typedef enum status_stop {
    stop_start,
    stop_mid,
    stop_end,
    stop_error
} status_stop;


typedef struct Route {
    char* name;
    char* time_arrival;
    char* time_depart;
    status_stop type;
    struct Route* next;
} Route;

typedef struct Vehicle {
    Route* routes;
    char* number;
    struct Vehicle* next;
} Vehicle;

status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
status_cmd command(char** arg_one, char** arg_two);
bool valid_time(char* time_str, int size);
void get_date(char* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds);
Vehicle* find_vehicle(char* number, Vehicle* head);
status_code create_vehicle(char* number, Vehicle** new);
int compare_time(char* a, char* b);
status_code add_route(Route** _routes, char* _name, char* _time_arrival, char* _time_depart, status_stop _type);
status_stop get_status_stop(char* str);
status_code add_vehicle(Vehicle** vehicles, Vehicle* add);
status_code read_from_file(char* argv[], int argc, Vehicle** vehicles);
status_code print_transports(Vehicle* vehicles, char* filename);
void free_routes(Route* routes);
void free_transports(Vehicle* vehicles);
int size_list(Route* routes);
int num_of_routes(Route* routes);
time_t get_time_difference_minutes(char *str1, char *str2);
status_code print_transport_info(Vehicle* vehicle, char* filename);
Vehicle* find_least_routes_vehicle(Vehicle* vehicles);
Vehicle* find_biggest_routes_vehicle(Vehicle* vehicles);
Vehicle* find_longest_routes_vehicle(Vehicle* vehicles);
Vehicle* find_fastest_routes_vehicle(Vehicle* vehicles);
Vehicle* find_fastest_stop_vehicle(Vehicle* vehicles);
Vehicle* find_longest_stop_vehicle(Vehicle* vehicles);

#endif