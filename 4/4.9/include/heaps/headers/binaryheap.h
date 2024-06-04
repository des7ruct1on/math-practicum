#ifndef binaryheap_h
#define binaryheap_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Binary_heap {
    Request* data;
    int capacity;
    int size;
} Binary_heap;

int binary_heap_size(Binary_heap* heap);
status_code create_queue_bh(Binary_heap** q);
void heapify_up_bh(Binary_heap* q);
status_code insert_bh(Logger* logger, Binary_heap* q, Request value);
void heapify_down_bh(Binary_heap* q);
Request* pop_bh(Binary_heap* q);
status_code merge_destruction_bh(Logger* logger, Binary_heap* a, Binary_heap* b);
status_code merge_no_destruction_bh(Logger* logger, Binary_heap* a, Binary_heap* b);
void free_binary(Binary_heap* heap);
#endif

