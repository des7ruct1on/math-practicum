#include "headers/fib.h"

status_code create_fib_node(fib_node** node, Request _req) {
    (*node) = (fib_node*)malloc(sizeof(fib_node));
    if (!*node) return code_error_alloc;
    (*node)->req = _req;
    (*node)->degree = 0;
    (*node)->parent = NULL;
    (*node)->child = NULL;
    (*node)->left = (*node)->right = (*node);
    (*node)->mark = 0;
    return code_success;
}

status_code create_fib_heap(Fibbonacci_heap** heap) {
    (*heap) = (Fibbonacci_heap*)malloc(sizeof(Fibbonacci_heap));
    if (!(*heap)) return code_error_alloc;
    (*heap)->root = NULL;
    (*heap)->count_nodes = 0;
    return code_success;
}

status_code insert_fib(Fibbonacci_heap** heap, Request _req) {
    status_code st_act;
    fib_node* tmp = NULL;
    st_act = create_fib_node(&tmp, _req);
    if (st_act  != code_success) return code_success;
    if (!(*heap)->root) {
        (*heap)->root = tmp;
    } else {
        tmp->left = (*heap)->root->left;
        tmp->right = (*heap)->root;
        (*heap)->root->left->right = tmp;
        (*heap)->root->left = tmp;
        if ((tmp->req.priority > (*heap)->root->req.priority)
        || ((tmp->req.priority == (*heap)->root->req.priority)
        && compare_time(&tmp->req.sending_time, &(*heap)->root->req.sending_time))) {
            (*heap)->root = tmp;
        }
    }
    (*heap)->count_nodes++;
    return code_success;
}

void free_fib_node(fib_node* node) {
    if (!node) return;
    fib_node* child = node->child;
    if (!child) {
        fib_node* tmp = child;
        do {
            fib_node* next = child->right;
            free_fib_node(child);
            child = next;
        } while (child != tmp);
    }
    free_request(&node->req);
    free(node);
}

void free_fib_heap(Fibbonacci_heap* heap) {
    if (!heap) return;
    free_fib_node(heap->root);
    free(heap);
}

void fib_merge(Fibbonacci_heap* A, Fibbonacci_heap* B) {
    if (!A->root) {
        A->root = B->root;
    } 
    else if (B->root) {
        fib_node* tmp = A->root->right;
        A->root->right = B->root->right;
        B->root->right->left = A->root;
        B->root->right = tmp;
        tmp->left = B->root;
        if (B->root->req.priority > A->root->req.priority ||
        (B->root->req.priority == A->root->req.priority
        && compare_time(&B->root->req.sending_time, &A->root->req.sending_time) < 0)) {
            A->root = B->root;
        }
    }
    A->count_nodes += B->count_nodes;
    B->count_nodes = 0;
}

Fibbonacci_heap* fib_merge_destr(Fibbonacci_heap* A, Fibbonacci_heap* B) {
    status_code st_act;
    Fibbonacci_heap* _new = NULL;
    st_act = create_fib_heap(&_new);
    if (st_act != code_success) return;
    if (!A->root) {
        A->root = B->root;
    } 
    else if (B->root) {
        fib_node* tmp = A->root->right;
        A->root->right = B->root->right;
        B->root->right->left = A->root;
        B->root->right = tmp;
        tmp->left = B->root;
        if (B->root->req.priority > A->root->req.priority ||
        (B->root->req.priority == A->root->req.priority
        && compare_time(&B->root->req.sending_time, &A->root->req.sending_time) < 0)) {
            A->root = B->root;
        }
    }
    A->count_nodes += B->count_nodes;
    B->count_nodes = 0;
    free(A);
    free(B);
    A = NULL;
    B = NULL;
    return _new;
}

void union_list(fib_node* first, fib_node* second) {
    if (!first || !second) return;

    fib_node* next_first = first->right;
    fib_node* prev_second = second->left;

    first->right = second;
    second->left = first;
    next_first->left = prev_second;
    prev_second->right = next_first;
}

status_code collect_nodes(Fibbonacci_heap* heap) {
    if (!heap || !heap->root) return;

    int max_degree_fib = log2(heap->count_nodes);
    fib_node** collection = (fib_node**)malloc(sizeof(fib_node*));
    if (!collection) return code_error_alloc;
    for (int i = 0; i < max_degree_fib; i++) collection[i] = NULL;

    fib_node* max = heap->root;
    fib_node* cur = heap->root->right;
    while (cur != heap->root) {
        int degree = cur->degree;
        fib_node* next = cur->right;
        while (collection[degree]) {
            fib_node* check = collection[degree];
            fib_node* to_add, *addition;
            if (check->req.priority > cur->req.priority) {
                to_add = check;
                addition = cur;
            } else {
                to_add = cur;
                addition = check;
            }
            union_list(to_add->child, addition);
            addition->parent = to_add;
            to_add->degree++;
            collection[degree] = NULL;
            degree++;
        }
        collection[degree] = cur;
    }
    for (int i = 0; i < max_degree_fib; i++) {
        if (collection[i] && collection[i]->req.priority > max->req.priority) {
            max = collection[i];
        }
    }
    heap->root = max;
    free(collection);
}

void find_max_fib(Fibbonacci_heap* heap, Request* res) {
    *res = heap->root->req;
}

void extract_max_fib(Fibbonacci_heap* heap, Request* res) {
    if (!heap || !heap->root) {
        res = NULL;
        return;
    }
    fib_node* max_node = heap->root;
    Request request = max_node->req;
    *res = request;
    if (max_node->right == max_node) {
        heap->root = NULL;
    } else {
        max_node->left->right = max_node->right;
        max_node->right->left = max_node->left;

        heap->root = max_node->right;
    }

    if (max_node->child) {
        fib_node* child = max_node->child;
        fib_node* tmp = child;

        do {
            fib_node* next = child->right;
            child->right = NULL;
            union_list(heap->root, child);
            child = next;
        } while(child != tmp);
    }
    free(max_node);
    max_node = NULL;
    collect_nodes(heap);
    heap->count_nodes--;
    return;
}

int size_fib_heap(Fibbonacci_heap* heap) {
    if (!heap) return 0;
    return heap->count_nodes;
}