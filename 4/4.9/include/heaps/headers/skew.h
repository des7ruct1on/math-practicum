#ifndef skew_h
#define skew_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Skew_heap {
    Request data;
    struct Skew_heap* left;
    struct Skew_heap* right;
} Skew_heap;

status_code create_skew_heap(Skew_heap** heap, Request req);
Skew_heap* skew_merge(Skew_heap* A, Skew_heap* B);
status_code get_max_skew(Skew_heap** heap, Request* find);
status_code insert_skew(Skew_heap** heap, Request req);
void free_skew(Skew_heap* heap);
Skew_heap* skew_merge_destroy(Skew_heap* A, Skew_heap* B);
int skew_heap_size(Skew_heap* heap);
#endif

