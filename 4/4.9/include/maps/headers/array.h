#ifndef array_h
#define array_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Array {
    int capacity;
    int size;
    char** keys;
    Post** posts;
    Post* min_load;
} Array;

status_code create_array(Array** arr, int _capacity);
status_code insert_array(Logger* logger, Array* arr, char* key, Post* office);
Post* search_array(Array* arr, char* key);
char* find_post_array(Array* arr, Post* find);
void free_array(Array* arr, void(*free_storage)(void*));
#endif