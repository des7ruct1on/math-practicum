#include "headers/skew.h"
#include <math.h>

status_code create_skew_heap(Skew_heap** heap, Request req) {
    (*heap) = (Skew_heap*)malloc(sizeof(Skew_heap));
    if (!*heap) return code_error_alloc;
    (*heap)->data = req;
    (*heap)->left = NULL;
    (*heap)->right = NULL;
    return code_success;
}

Skew_heap* skew_merge(Logger* logger, Skew_heap* A, Skew_heap* B) {
    if (!A) return B;
    if (!B) return A;
    if ((A->data.priority > B->data.priority) 
    ||( (A->data.priority == B->data.priority)  &&  (compare_time(&A->data.sending_time, &B->data.sending_time) < 0))) {
        Skew_heap* tmp = A->left;
        A->left = A->right;
        A->right = skew_merge(logger, tmp, B);
        return A;
    } else {
        Skew_heap* tmp = B->left;
        B->left = B->right;
        B->right = skew_merge(logger, tmp, A);
        return B;
    }
     
}

status_code get_max_skew(Logger* logger, Skew_heap** heap, Request* find) {
    if (!*heap) return code_invalid_parameter;
    Request max_req = (*heap)->data;
    Skew_heap* tmp = (*heap);
    (*heap) = skew_merge(logger, (*heap)->left, (*heap)->right);
    free(tmp);
    *find = max_req;
    return code_success;
}

status_code insert_skew(Logger* logger, Skew_heap** heap, Request req) {
    Skew_heap* _new = NULL;
    status_code st_act;
    st_act = create_skew_heap(&_new, req);
    if (st_act != code_success) {
        create_log(&logger, "error after creating heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    (*heap) = skew_merge(logger, *heap, _new);
    return code_success;
}

void free_skew(Skew_heap* heap) {
    if (!heap) return;
    free_skew(heap->left);
    free_skew(heap->right);
    free_request(&(heap->data));
    free(heap);
}

Skew_heap* skew_merge_destroy(Logger* logger, Skew_heap* A, Skew_heap* B) {
    if (!A) {
        free(B);
        return NULL;
    }
    if (!B) {
        free(A);
        return NULL;
    }
    if ((A->data.priority > B->data.priority) || ( (A->data.priority == B->data.priority)  &&  (compare_time(&A->data.sending_time, &B->data.sending_time) < 0))) {
        Skew_heap* tmp = A;
        A = B;
        B = tmp;
    } 
    Skew_heap* tmp = skew_merge_destroy(logger, A->right, B);
    A->right = A->left;
    A->left = tmp;
    free(B);
    return A;
}

int skew_heap_size(Skew_heap* heap) {
    if (!heap) {
        return 0;
    }
    return 1 + skew_heap_size(heap->left) + skew_heap_size(heap->right);
}
