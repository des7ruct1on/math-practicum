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
#include "string.h"

typedef struct Adress {
    My_string* city;
    My_string* street;
    unsigned int house;
    My_string* block;
    unsigned int flat;
    My_string* index;
} Adress;

typedef struct Mail {
    Adress ad;
    double weight;
    My_string* id;
    My_string* time_create;
    My_string* time_get;
} Mail;

typedef struct Post {
    Adress* cur_id;
    Mail* mails;
    int size;
    int capacity;
} Post;

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
status_code create_adress(Adress** new_adress, const My_string* info);
status_code create_mail(Mail** new_mail, const My_string* info);
status_code create_post(Post** new_post, int _capacity, const My_string* info);
status_code add_mail(Post** cur_post, const My_string* info);
bool check_equal(Adress* left, Adress* right);
int free_adress(Adress* tmp);
int free_mail(Mail* tmp);
Post* find_post(Post* posts, const My_string* id);
void get_date(My_string* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds);
Mail* find_mail(Post* _mails, My_string id, int size);
int compare_mails(const void* a, const void* b);
#endif