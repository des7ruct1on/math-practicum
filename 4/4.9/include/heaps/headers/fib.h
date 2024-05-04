#ifndef fib_h
#define fib_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct fib_node {
    Request req;
    int degree;
    struct fib_node* parent;
    struct fib_node* child;
    struct fib_node* left;
    struct fib_node* right;
    int mark;
} fib_node;

typedef struct Fibbonacci_heap {
    fib_node* root;
    int count_nodes;
} Fibbonacci_heap;

status_code create_fib_node(fib_node** node, Request _req);
status_code create_fib_heap(Fibbonacci_heap** heap);
status_code insert_fib(Logger* logger, Fibbonacci_heap** heap, Request _req);
void free_fib_node(fib_node* node);
void free_fib_heap(Fibbonacci_heap* heap);
void fib_merge(Logger* logger, Fibbonacci_heap* A, Fibbonacci_heap* B);
Fibbonacci_heap* fib_merge_destr(Logger* logger, Fibbonacci_heap* A, Fibbonacci_heap* B);
void union_list(fib_node* first, fib_node* second);
status_code collect_nodes(Fibbonacci_heap* heap);
void find_max_fib(Fibbonacci_heap* heap, Request* res);
void extract_max_fib(Fibbonacci_heap* heap, Request* res);
int size_fib_heap(Fibbonacci_heap* heap);
#endif

