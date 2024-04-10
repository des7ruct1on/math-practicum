#ifndef binomial_h
#define binomial_h

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "../../utils/headers/struct.h"
#include "../../utils/headers/logger.h"

typedef struct Binom_node {
    Request data;
    int degree;
    struct Binom_node* child;
    struct Binom_node* sibling;
} Binom_node;

typedef struct Binomial_heap {
    Binom_node* root;
    int size;
} Binomial_heap;

void free_binom_node(Binom_node* heap);
void free_binom_heap(Binomial_heap* heap);
status_code create_binom_heap(Binomial_heap** heap);
status_code binom_node_copy(Binom_node** copied, Binom_node* to_copy);
status_code binom_heap_copy(Binomial_heap** copied, Binomial_heap* to_copy);
status_code merge_binom(Binomial_heap** result, Binomial_heap* A, Binomial_heap* B);
status_code merge_binom_no_destruct(Binomial_heap** result, Binomial_heap* A, Binomial_heap* B);
int size_binom_heap(Binomial_heap* heap);
status_code find_binomial_heap(Binomial_heap* heap, Binom_node** prev, Binom_node** find);
status_code get_binom_request(Binomial_heap* heap, Request* req);
status_code pop_binom_heap(Binomial_heap* heap, Request* req);
status_code create_binom_node(Binom_node** node, Request req);
status_code insert_binomial(Binomial_heap* heap, Request req);
#endif

