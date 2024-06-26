#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "headers/struct.h"


status_code request_copy(Request a, Request** b) {
    (*b) = (Request*)malloc(sizeof(Request));
    if (!(*b)) return code_error_alloc;
    (*b)->index = strdup(a.index);
    (*b)->id = strdup(a.id);
    (*b)->priority = a.priority;
    (*b)->sending_time.day = a.sending_time.day;
    (*b)->sending_time.month = a.sending_time.month;
    (*b)->sending_time.minute = a.sending_time.minute;
    (*b)->sending_time.year = a.sending_time.year;
    (*b)->sending_time.second = a.sending_time.second;
    (*b)->sending_time.hour = a.sending_time.hour;
    (*b)->text = strdup(a.text);
    return code_success;
}

void swap_request(Request* a, Request* b) {
    Request tmp = *a;
    *a = *b;
    *b = tmp;
}

void free_request(Request* a) {
    if (!a) return;
    free(a->id);
    free(a->text);
    free(a->index);
    free(a);
}

int compare_time(my_time *time1, my_time *time2) {
    if (time1->year != time2->year) {
        return time1->year < time2->year ? -1 : 1;
    }
    if (time1->month != time2->month) {
        return time1->month < time2->month ? -1 : 1;
    }
    if (time1->day != time2->day) {
        return time1->day < time2->day ? -1 : 1;
    }
    if (time1->hour != time2->hour) {
        return time1->hour < time2->hour ? -1 : 1;
    }
    if (time1->minute != time2->minute) {
        return time1->minute < time2->minute ? -1 : 1;
    }
    return 0;
}

status_code get_iso_time(char* str, my_time* _time) {
    if (!str || !_time) return code_invalid_parameter;

    // Удалить символ новой строки, если он есть
    str[strcspn(str, "\n")] = '\0';
    printf("            VREMYA: %s\n", str);

    if (sscanf(str, "%04d-%02d-%02dT%02d:%02d:%02d", &((*_time).year), &((*_time).month), &((*_time).day), &((*_time).hour), &((*_time).minute), &((*_time).second)) != 6) {
        return code_invalid_parameter; 
    }
    printf("        >>> %04d-%02d-%02dT%02d:%02d:%02d", (*_time).year, (*_time).month, (*_time).day, (*_time).hour, (*_time).minute, (*_time).second);
    return code_success;
}


status_code create_model(Model** model, Heap h, Storage s, void* _map) {
    if (!_map)  return code_invalid_parameter;
    (*model) = (Model*)malloc(sizeof(Model));
    if (!*model) return code_error_alloc;
    (*model)->heap_type = h;
    (*model)->storage_type = s;
    (*model)->map = _map;
    return code_success;
}

void get_unique_id(char* operator_name) {
    const char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"; // Алфавит
    int alphabet_length = strlen(alphabet);

   
    for (int i = 0; i < 32; i++) {
        operator_name[i] = alphabet[rand() % 32]; 
    }
    operator_name[32] = '\0'; 
}

status_code create_request(Request* req, char* line) {
    printf("\n\n%s\n", line);
    if (!line) return code_invalid_parameter;
    status_code st_act;
    char* token = NULL;
    //char input_string[] = "2024-04-04T12:34:57 3 129337 \"поменяй\"";
    token = strtok(line, " "); // Получаем первое слово
    printf("    TIME: %s\n", token);
    st_act = get_iso_time(token, &req->sending_time);
    printf("\n\n\n\n%s\n\n\n\n", token);
    printf("%d %d %d - %d %d %d \n", req->sending_time.year, req->sending_time.month, req->sending_time.day, req->sending_time.hour, req->sending_time.minute, req->sending_time.second);
    if (st_act != code_success) return st_act;
    token = strtok(NULL, " ");
    int prior;
    sscanf(token, "%d", &prior);
    req->priority = prior;
    printf("    prior: %d\n", prior);
    token = strtok(NULL, " ");
    req->index = strdup(token);
    printf("    index: %s\n", token);
    token = strtok(NULL, " ");
    req->text = strdup(token);
    printf("    text: %s\n", token);
    char tmp[256];
    get_unique_id(tmp);
    req->id = strdup(tmp);
    printf("    id: %s\n", req->id );
    return code_success;

}

void add_minute(my_time *time) {
    time->minute += 1;
    if (time->minute >= 60) {
        time->minute = 0;
        time->hour += 1;
        if (time->hour >= 24) {
            time->hour = 0;
            time->day += 1;
            if (time->day >= 31) {
                time->day = 1;
                time->month += 1;

                if (time->month >= 13) {
                    time->month = 1;
                    time->year += 1;
                }
            }
        }
    }
}

void add_minutes(my_time *time, int val) {
    time->minute += val;
    if (time->minute >= 60) {
        time->minute = 0;
        time->hour += 1;
        if (time->hour >= 24) {
            time->hour = 0;
            time->day += 1;
            if (time->day >= 31) {
                time->day = 1;
                time->month += 1;

                if (time->month >= 13) {
                    time->month = 1;
                    time->year += 1;
                }
            }
        }
    }
}

my_time get_time_now() {
    my_time cur_time; 
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    cur_time.day = now->tm_mday;
    cur_time.month = now->tm_mon + 1;
    cur_time.year = now->tm_year + 1900;
    cur_time.hour = now->tm_hour;
    cur_time.minute = now->tm_min;
    cur_time.second = now->tm_sec;
    return cur_time;
}

int time_difference_minutes(my_time start_time, my_time end_time) {
    unsigned int start_minutes = start_time.day * 24 * 60 + start_time.hour * 60 + start_time.minute;
    unsigned int end_minutes = end_time.day * 24 * 60 + end_time.hour * 60 + end_time.minute;
    printf("%u ---- %u\n", start_minutes, end_minutes);
    int difference_minutes = end_minutes - start_minutes;
    return difference_minutes;
}