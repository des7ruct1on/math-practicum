#ifndef leftistheap_h
#define leftistheap_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Leftist_heap {
    Request key;
    int dist;
    struct Leftist_heap* left;
    struct Leftist_heap* right;
} Leftist_heap;

void free_leftist(Leftist_heap* heap);
status_code create_heap_lh(Leftist_heap** heap, Request key);
Leftist_heap* copy_lh(Logger* logger, Leftist_heap *src);
void swap_lh(Leftist_heap** a, Leftist_heap** b);
Leftist_heap* merge_lh(Leftist_heap* x, Leftist_heap* y);
status_code insert_lh(Logger* logger, Leftist_heap** heap, Request key);
Request* pop_lh(Leftist_heap** heap);
int leftist_heap_size(Leftist_heap* heap);
#endif

