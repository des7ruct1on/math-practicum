#include "headers/binomial.h"

void free_binom_node(Binom_node* heap) {
    if (!heap) return;
    free_binom_node(heap->child);
    free_binom_node(heap->sibling);
    free_request(&heap->data);
    free(heap);
    heap = NULL;
}

void free_binom_heap(Binomial_heap* heap) {
    if (!heap) return;
    free_binom_node(heap->root);
    free(heap);
    heap = NULL;
}

status_code create_binom_heap(Binomial_heap** heap) {
    (*heap) = (Binomial_heap*)malloc(sizeof(Binomial_heap));
    if (!*heap)  return code_error_alloc;
    (*heap)->root = NULL;
    (*heap)->size = 0;
    return code_success;
}

status_code binom_node_copy(Binom_node** copied, Binom_node* to_copy) {
    if (!to_copy) {
        *copied = NULL;
        return code_success;
    }
    *copied = (Binom_node*)malloc(sizeof(Binom_node));
    if (!*copied) return code_error_alloc;

    status_code st_act;
    st_act = request_copy(to_copy->data, &(*copied)->data);
    if (st_act != code_success) {
        free(*copied);
        return st_act;
    }
    (*copied)->degree = to_copy->degree;
    st_act = binom_node_copy(&(*copied)->child, to_copy->child);
    if (st_act != code_success) {
        free_binom_node(*copied);
        return st_act;
    }
    st_act = binom_node_copy(&(*copied)->sibling, to_copy->sibling);
    if (st_act != code_success) {
        free_binom_node(*copied);
        return st_act;
    }
    return code_success;
}

status_code binom_heap_copy(Binomial_heap** copied, Binomial_heap* to_copy) {
    if (!to_copy) {
        *copied = NULL;
        return code_success;
    }
    (*copied)->size = to_copy->size;
    status_code st_act;
    st_act = binom_node_copy(&(*copied)->root, to_copy->root);
    if (st_act != code_success) {
        free_binom_heap(*copied, 1);
        return st_act;
    }
    return code_success;
}

status_code merge_binom(Binomial_heap** result, Binomial_heap* A, Binomial_heap* B) {
    status_code st_act;
    if (!B) {
        *result = A;
        return st_act;
    }
    if (!A) {
        *result = B;
        return st_act;
    }
    Binom_node* left_ptr = A->root;
    Binom_node* right_ptr = B->root;

    if (left_ptr && (!right_ptr || left_ptr->degree < right_ptr->degree)) {
        (*result)->root = left_ptr;
        left_ptr = left_ptr->sibling;
    } else {
        (*result)->root = right_ptr;
        right_ptr = right_ptr->sibling;
    }

    Binom_node* tmp = (*result)->root;

    while (left_ptr && right_ptr) {
        if (left_ptr->degree < right_ptr->degree) {
            tmp->sibling = left_ptr;
            left_ptr = left_ptr->sibling;
        } else {
            tmp->sibling = right_ptr;
            right_ptr = right_ptr->sibling;
        }
        tmp = tmp->sibling;
    }

    while (left_ptr) {
        tmp->sibling = left_ptr;
        left_ptr = left_ptr->sibling;
        tmp = tmp->sibling;
    }
    while (right_ptr) {
        tmp->sibling = right_ptr;
        right_ptr = right_ptr->sibling;
        tmp = tmp->sibling;
    }

    Binom_node* prev = NULL;
    Binom_node* cur = (*result)->root;
    Binom_node* next = (*result)->root->sibling;

    while (next) {
        Binom_node* tmp_cur = next->sibling;
        if (cur->degree == next->degree && (!tmp_cur || tmp_cur->degree != next->degree)) {
            if (compare_time(&cur->data.sending_time, &next->data.sending_time) <= 0) {
                next->sibling = cur->child;
                cur->child = next;
            } else {
                cur->sibling = next->child;
                next->child = cur;
                if (cur == (*result)->root) {
                    (*result)->root = next;
                }
                cur = next;
            }
            if (prev) {
                prev->sibling = cur;
            }
            cur->degree++;
            cur->sibling = tmp_cur;
            next = tmp_cur;
        } else {
            prev = cur;
            cur = next;
            next = next->sibling;
        }
    }
    (*result)->size = A->size + B->size;
    A->root = NULL;
    B->root = NULL;
    return code_success;
}

status_code merge_binom_no_destruct(Binomial_heap** result, Binomial_heap* A, Binomial_heap* B) {
    Binomial_heap* left_copy, *right_copy;
    status_code st_act;
    st_act = binom_heap_copy(&left_copy, A);
    if (st_act != code_success) {
        return st_act;
    }
    st_act = binom_heap_copy(&right_copy, B);
    if (st_act != code_success) {
        free_binom_heap(left_copy, 1);
        return st_act;
    }
    st_act = merge_binom(result, left_copy, right_copy);
    if (st_act != code_success) {
        free_binom_heap(left_copy, 1);
        free_binom_heap(right_copy, 1);
        return st_act;
    }
    return code_success;
}

int size_binom_heap(Binomial_heap* heap) {
    if (!heap) return 0;
    return heap->size;
}

status_code find_binomial_heap(Binomial_heap* heap, Binom_node** prev, Binom_node** find) {
    if (!heap->root) {
        *find = NULL;
        return code_success;
    }
    Binom_node* _prev = NULL;
    Binom_node* cur = heap->root;
    Binom_node* tmp = heap->root;

    while(tmp->sibling) {
        if (compare_time(&tmp->data.sending_time, &cur->data.sending_time) < 0) {
            _prev = tmp;
            cur = tmp->sibling;
        }
        tmp = tmp->sibling;
    }

    if (*prev) {
        *prev = _prev;
    }
    *find = cur;
    return code_success;
}

status_code get_binom_request(Binomial_heap* heap, Request* req) {
    if (!heap) return code_invalid_parameter;
    Binom_node* tmp = NULL;
    status_code st_act;
    st_act = find_binomial_heap(heap, NULL, &tmp);
    if (st_act != code_success) return st_act;

    if (!tmp) {
        req = NULL;
        return code_success;
    }
    *req = tmp->data;
    return code_success;
}

status_code pop_binom_heap(Binomial_heap* heap, Request* req) {
    if (!heap) return code_invalid_parameter;
    Binom_node* prev = NULL;
    Binom_node* tmp = NULL;
    status_code st_act;
    st_act = find_binomial_heap(heap, &prev, &tmp);
    if (st_act != code_success) return st_act;

    if (!tmp) {
        req = NULL;
        return code_success;
    }

    int tmp_size = pow(2, tmp->degree);
    if (req) {
        *req = tmp->data;
    }
    if (prev) {
        prev->sibling = tmp->sibling;
    }
    if (tmp == heap->root) {
        heap->root = tmp->sibling;
    }

    Binomial_heap* add = NULL;
    st_act = create_binom_heap(&add);
    if (st_act != code_success) return st_act;
    add->root = tmp->child;
    free(tmp);

    prev = NULL;
    tmp = add->root;
    while (tmp) {
        Binom_node* next = tmp->sibling;
        tmp->sibling = prev;
        prev = tmp;
        tmp = next;
    }
    add->root = prev;
    heap->size -= tmp_size;
    add->size = tmp_size - 1;
    st_act = merge_binom(&heap, heap, add);
    return code_success;
}

status_code create_binom_node(Binom_node** node, Request req) {
    (*node) = (Binom_node*)malloc(sizeof(Binom_node));
    if (!*node) return code_error_alloc;
    (*node)->child = NULL;
    (*node)->sibling = NULL;
    (*node)->data = req;
    (*node)->degree = 0;
    return code_success;
}

status_code insert_binomial(Binomial_heap* heap, Request req) {
    status_code st_act;
    if (!heap) {
        st_act = create_binom_heap(&heap);
        if (st_act != code_success) return st_act;
    }
    Binom_node* add = NULL;
    st_act = create_binom_node(&add, req);
    if (st_act != code_success) {
        free_binom_heap(heap, 1);
        return st_act;
    }
    Binomial_heap* addition = NULL;
    st_act = create_binom_heap(&heap);
    if (st_act != code_success) {
        free_binom_heap(heap, 1);
        free_binom_node(add);
        return st_act;
    }
    st_act = merge_binom(&heap, heap, addition);
    if (st_act != code_success) {
        free_binom_heap(heap, 1);
        free_binom_node(add);
        return st_act;
    }
    return code_success;
}