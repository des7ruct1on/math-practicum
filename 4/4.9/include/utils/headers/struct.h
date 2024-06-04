#ifndef struct_h
#define struct_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct my_time {
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
} my_time;

typedef struct Request {
    my_time sending_time;
    int priority;
    char* id;
    char* index;
    char* text;
} Request;

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc,
    code_error_oppening
} status_code;

typedef struct Operator {
    my_time start;
    char name[256];
    int time_process;
    Request* current_request;
} Operator;

typedef struct Post {
    void* storage;
    int size_operators;
    int free_requests;
    bool is_overload;
    Operator* ops;
} Post;

typedef enum Heap {
    binary,
    binomial,
    fibonacci,
    skew,
    treap,
    leftist
} Heap;

typedef enum Storage {
    array,
    bst,
    trie,
    hashset
} Storage;

typedef struct Model {
    Heap heap_type;
    Storage storage_type;
    void* map;
    my_time start_time;
    my_time end_time;
    int min_process_time;
    int max_process_time;
    int count_post;
    double coeff_overload;
} Model;

void free_request(Request* a);
status_code request_copy(Request a, Request** b);
void swap_request(Request* a, Request* b);
int compare_time(my_time *time1, my_time *time2);
status_code get_iso_time(char* str, my_time* _time);
status_code create_model(Model** model, Heap h, Storage s, void* _map);
status_code create_request(Request* req, char* line);
void add_minute(my_time *time);
void get_unique_id(char* operator_name);
my_time get_time_now();
void add_minutes(my_time *time, int val);
int time_difference_minutes(my_time start_time, my_time end_time);
#endif