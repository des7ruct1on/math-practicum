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
    (*b)->id = a.id;
    (*b)->id = strdup(a.id);
    (*b)->priority = a.id;
    (*b)->sending_time.day = a.sending_time.day;
    (*b)->sending_time.month = a.sending_time.month;
    (*b)->sending_time.minute = a.sending_time.minute;
    (*b)->sending_time.year = a.sending_time.year;
    (*b)->sending_time.second = a.sending_time.second;
    (*b)->sending_time.hour = a.sending_time.hour;
    (*b)->text = strdup(a.text);
    return code_success;
}

void swap_request(Request** a, Request** b) {
    Request* tmp = *a;
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
    if (time1->second != time2->second) {
        return time1->second < time2->second ? -1 : 1;
    }
    return 0;
}

status_code get_iso_time(char* str, my_time* _time) {
    if (!str || !_time) return code_invalid_parameter;

    if (sscanf(str, "%d-%d-%dT%d:%d:%d", &(_time->year), &(_time->month), &(_time->day), &(_time->hour), &(_time->minute), &(_time->second)) != 6) {
        return code_invalid_parameter; 
    }

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

    srand(time(NULL)); // Инициализация генератора псевдослучайных чисел
    for (int i = 0; i < 32; i++) {
        operator_name[i] = alphabet[rand() % 32]; 
    }
    operator_name[32] = '\0'; 
}

status_code create_request(Request* req, char* line) {
    if (!line) return code_invalid_parameter;
    status_code st_act;
    char* token = NULL;
    //char input_string[] = "2024-04-04T12:34:57 3 129337 \"поменяй\"";
    token = strtok(line, " "); // Получаем первое слово
    st_act = get_iso_time(token, &req->sending_time);
    if (st_act != code_success) return st_act;
    token = strtok(NULL, " ");
    int prior;
    sscanf(token, "%d", &prior);
    req->priority = prior;
    token = strtok(NULL, " ");
    req->index = strdup(token);
    token = strtok(NULL, " ");
    req->text = strdup(token);
    char tmp[256];
    get_unique_id(tmp);
    req->id = strdup(tmp);
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