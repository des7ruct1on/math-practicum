#include "headers/skew.h"

typedef struct Skew_heap {
    Request data;
    struct Skew_heap* left;
    struct Skew_heap* right;
} Skew_heap;

status_code create_skew_heap(Skew_heap** heap, Request req) {
    (*heap) = (Skew_heap*)malloc(sizeof(Skew_heap));
    if (!*heap) return code_error_alloc;
    (*heap)->data = req;
    (*heap)->left = NULL;
    (*heap)->right = NULL;
    return code_success;
}

Skew_heap* skew_merge(Skew_heap* A, Skew_heap* B) {
    if (!A) return B;
    if (!B) return A;
    if ((A->data.priority > B->data.priority) 
    ||( (A->data.priority == B->data.priority)  &&  (compare_time(&A->data.sending_time, &B->data.sending_time) < 0))) {
        Skew_heap* tmp = A->left;
        A->left = A->right;
        A->right = skew_merge(tmp, B);
        return A;
    } else {
        Skew_heap* tmp = B->left;
        B->left = B->right;
        B->right = skew_merge(tmp, A);
        return B;
    }
     
}

status_code get_max_skew(Skew_heap** heap, Request* find) {
    if (!*heap) return code_invalid_parameter;
    Request max_req = (*heap)->data;
    Skew_heap* tmp = (*heap);
    (*heap) = skew_merge((*heap)->left, (*heap)->right);
    free(tmp);
    *find = max_req;
    return code_success;
}

status_code insert_skew(Skew_heap** heap, Request req) {
    Skew_heap* _new = NULL;
    status_code st_act;
    st_act = create_skew_heap(&_new, req);
    (*heap) = skew_merge(*heap, _new);
    return code_success;
}

void free_skew(Skew_heap* heap) {
    if (!heap) return;
    free_skew(heap->left);
    free_skew(heap->right);
    free_request(&(heap->data));
    free(heap);
}

Skew_heap* skew_merge_destroy(Skew_heap* A, Skew_heap* B) {
    if (!A) {
        free(B);
        return NULL;
    if (!B) {
        free(A);
        return NULL;
    }
    if ((A->data.priority > B->data.priority) || ( (A->data.priority == B->data.priority)  &&  (compare_time(&A->data.sending_time, &B->data.sending_time) < 0))) {
        Skew_heap* tmp = A;
        A = B;
        B = tmp;
    } 
    Skew_heap* tmp = skew_merge_destroy(A->right, B);
    A->right = A->left;
    A->left = tmp;
    free(B);
    return A;
    }
}

int skew_heap_size(Skew_heap* heap) {
    if (!heap) {
        return 0;
    }
    return 1 + skew_heap_size(heap->left) + skew_heap_size(heap->right);
}
