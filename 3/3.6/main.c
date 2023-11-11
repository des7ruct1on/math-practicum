#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

void print_menu() {
    printf("|================================================================================|\n");
    printf("| [Exit] - to exit the programm                                                  |\n");
    printf("| [Routes] <Max / Min> <File> - to find out vehicle with the max / min of routes |\n");
    printf("| [Time] <Max / Min> <File> - to find vehicle with the longest / fastest route   |\n");
    printf("| [Stop] <Max / Min> <File> - to find vehicle with the longest / fastest stop    |\n");
    printf("| [Print] <File>- to print all routes                                            |\n");
    printf("|================================================================================|\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your program must include at least 1 file!!!\n");
        return 1;
    }
    char* arg_one = NULL;
    char* arg_two = NULL;
    Vehicle* storage = NULL;
    status_code st_action;
    status_cmd st_act;
    st_action = read_from_file(argv, argc, &storage);
    if (st_action == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        return -1;
    } else if (st_action == code_error_oppening) {
        printf("Can`t open file!\n");
        return -2;
    } else if (st_action == code_invalid_parameter) {
        printf("Invalid parameter detected!\n");
        return -3;
    }
    Vehicle* tmp = NULL;
    while (true) {
        print_menu();
        st_act = command(&arg_one, &arg_two);
        switch (st_act) {
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!\n");
                break;
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_print:
                st_action = print_transports(storage, arg_one);
                if (st_action == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_action == code_invalid_parameter) {
                    printf("Invalid parameter detected!\n");
                } else if (st_action == code_error_oppening) {
                    printf("Can`t open file!\n");
                }
                break;
            case cmd_route:
                if (!strcmp(arg_one, "Max")) {
                    tmp = find_biggest_routes_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                    break;
                } else if (!strcmp(arg_one, "Min")) {
                    tmp = find_least_routes_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                } else {
                    printf("Invalid parameter detected!\n");
                }
                break;
            case cmd_stop:
                if (!strcmp(arg_one, "Max")) {
                    tmp = find_longest_stop_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                    break;
                } else if (!strcmp(arg_one, "Min")) {
                    tmp = find_fastest_stop_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                    break;
                } else {
                    printf("Invalid parameter detected!\n");
                    break;
                }
            case cmd_time:
                if (!strcmp(arg_one, "Max")) {
                    tmp = find_longest_routes_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                    break;
                } else if (!strcmp(arg_one, "Min")) {
                    tmp = find_fastest_routes_vehicle(storage);
                    st_action = print_transport_info(tmp, arg_two);
                    if (st_action == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_action == code_invalid_parameter) {
                        printf("Invalid parameter detected!\n");
                    }
                    break;
                } else {
                    printf("Invalid parameter detected!\n");
                    break;
                }
            case cmd_exit:
                break;
        }
        if (arg_one) free(arg_one); arg_one = NULL;
        if (arg_two) free(arg_one); arg_two = NULL;
        if (st_act == cmd_exit || st_act == cmd_error_alloc) break;
    }
    free_transports(storage);
    return 0;
}