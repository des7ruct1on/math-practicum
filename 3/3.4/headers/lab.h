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
    int house;
    My_string* block;
    int flat;
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

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_sort,
    cmd_create_post,
    cmd_change_post,
    cmd_add_mail,
    cmd_remove_mail,
    cmd_print_expired,
    cmd_print_non_expired,
    cmd_table
} status_cmd;

status_code free_storage(Post** posts, int size);
status_code add_post_storage(Post*** new_post, Post* new, int* capacity, int* size);
status_cmd command(char** arg_one, char** arg_two, My_string** info);
status_code create_adress(Adress** new_adress, const My_string* info);
status_code create_mail(Mail** new_mail, const My_string* info);
status_code create_post(Post** new_post, const My_string* info);
status_code add_mail(Post** cur_post, const My_string* info);
status_code remove_mail(Post* cur_post, const My_string* id);
bool check_equal(Adress* left, Adress* right);
int free_adress(Adress* tmp);
int free_mail(Mail* tmp);
Post* find_post(Post** posts, const My_string* id, int size);
void get_date(My_string* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds);
Mail* find_mail(Post* _mails, My_string id, int size);
int compare_mails(const void* a, const void* b);
status_code find_expired_mails(Post* post, Mail*** exp_mails, int* size);
status_code find_non_expired_mails(Post* post, Mail*** exp_mails, int* size);
int is_expired(const Mail* a) ;
int compare_mails_date_create(const void* a, const void* b);
void print_mails(Mail* mails, int size);
#endif