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
} Post;
#endif