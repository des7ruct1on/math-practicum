#include "headers/leftistheap.h"
void free_leftist(Leftist_heap* heap) {
    if (!heap) return;
    free_request(&heap->key);
    free_leftist(heap->left);
    free_leftist(heap->right);
    free(heap);
}

status_code create_heap_lh(Leftist_heap** heap, Request key) {
    (*heap) = (Leftist_heap*)malloc(sizeof(Leftist_heap));
    if (!*heap) return code_error_alloc;
    (*heap)->left = (*heap)->right = NULL;
    (*heap)->dist = 0;
    (*heap)->key = key;
    return code_success;
}

Leftist_heap* copy_lh(Leftist_heap *src) {
    if (!src) {
        return NULL;
    }
    Leftist_heap* res = NULL;
    status_code st_act;
    st_act = create_heap(&res, src->key);
    if (st_act != code_success) return NULL;
    res->dist = src->dist;
    res->left = copy_lh(src->left);
    res->right = copy_lh(src->right);
    return res;
}

void swap_lh(Leftist_heap** a, Leftist_heap** b) {
    Leftist_heap* tmp = *a;
    *a = *b;
    *b = tmp;
}


Leftist_heap* merge_lh(Leftist_heap* x, Leftist_heap* y) {
    if (!x) return y;
    if (!y)  return x;
    if (x->key.priority > y->key.priority) {
        swap_lh(&x, &y);
    }
    Leftist_heap* tmp = merge_lh(x->right, y);
    //free(x->right);
    x->right = tmp;
    if (!x->left || (x->right && x->left->dist < x->right->dist)) {
        swap_lh(&(x->left) , &(x->right));
    }
    if (!x->right) {
        x->dist = 0;
    } else {
        x->dist = x->right->dist + 1;
    }
    return x;
}

status_code insert_lh(Leftist_heap** heap, Request key) {
    Leftist_heap* node = NULL;
    status_code st_act;
    st_act = create_heap_lh(&node, key);
    if (st_act != code_success) return st_act;
    *heap = merge_lh(*heap, node);
    return code_success;
}

Request* pop_lh(Leftist_heap** heap) {
    if (!*heap) return NULL;
    Request* val = &(*heap)->key;
    Leftist_heap* left = (*heap)->left;
    Leftist_heap* right = (*heap)->right;
    free(*heap);
    *heap = NULL;
    *heap = merge_lh(left, right);
    return val;
}

int leftist_heap_size(Leftist_heap* heap) {
    if (!heap) {
        return 0;
    } else {
        return 1 + leftist_heap_size(heap->left) + leftist_heap_size(heap->right);
    }
}


