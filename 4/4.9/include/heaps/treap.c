#include "headers/treap.h"
#include <math.h>

Treap* merge_treap(Logger* logger, Treap* a, Treap* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->data.priority > b->data.priority) {
        a->right = merge_treap(logger, a->right, b);
        return a;
    } else {
        b->left = merge_treap(logger, a, b->left);
        return b;
    }
}

status_code treap_copy(Logger* logger, Treap** copy, Treap* add) {
    if (!add) {
        *copy = NULL;
        return code_success;
    }
    *copy = (Treap*)malloc(sizeof(Treap));
    if (!*copy) {
        return code_error_alloc;
    }
    
    status_code st_act;
    Request* tmp = NULL;
    st_act = request_copy(add->data, &tmp);
    if (st_act != code_success) {
        create_log(&logger, "error after copying request, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free_treap(*copy);
    } else {
        (*copy)->data = *tmp;
    }
    st_act = treap_copy(logger, &(*copy)->left, add->left);
    if (st_act != code_success) {
        create_log(&logger, "error after copying left node, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free_treap(*copy);
    }
    st_act = treap_copy(logger, &(*copy)->right, add->right);
    if (st_act != code_success) {
        create_log(&logger, "error after copying right node, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free_treap(*copy);
    }
    return code_success;
}

void free_treap(Treap* tree) {
    if (!tree) return;
    free_request(&tree->data);
    free_treap(tree->left);
    free_treap(tree->right);
    free(tree);
}

Treap* merge_treap_copy(Logger* logger, Treap* a, Treap* b) {
    if (!a && !b) return NULL;
    Treap* lc = NULL;
    Treap* rc = NULL;
    status_code st_act;
    st_act = treap_copy(logger, &lc, a);
    if (st_act != code_success) {
        create_log(&logger, "error after copying left node, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return NULL;
    }
    st_act = treap_copy(logger, &rc, b);
    if (st_act != code_success) {
        create_log(&logger, "error after copying right node, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free_treap(a);
        return NULL;
    }
    return merge_treap(logger, lc, rc);
}

void split_treap(Treap* root, Request* key, Treap** a, Treap** b) {
    if (!root) {
        *a = *b = NULL;
        return;
    }
    if (strcmp(root->data.id, key->id) < 0) {
        split_treap(root->right, key, &root->right, b);
        *a = root;
        return;
    } else {
        split_treap(root->left, key, a, &root->left);
        *b = root;
        return;
    }
}

Treap* create_node_treap(Request tmp) {
    Treap* node = (Treap*)malloc(sizeof(Treap));
    node->data = tmp;
    node->left = node->right = NULL;
    return node;
}

void insert_treap(Logger* logger, Treap** root, Request tmp) {
    Treap* less, *greater;
    split_treap(*root, &(*root)->data, &less, &greater);
    Treap* add = create_node_treap(tmp);
    *root = merge_treap(logger, merge_treap(logger, less, add), greater);
}

Treap* search_treap(Logger* logger, Treap* root, Request* tmp) {
    status_code st_act;
    Treap* less, *equal, *greater;
    split_treap(root, tmp, &less, &greater);
    Request* t = NULL;
    st_act = request_copy(*tmp, &t);
    if (st_act != code_success) {
        free_request(t);
        root = merge_treap(logger, less, greater);
        return NULL;
    }
    strcat(t->id, "z");
    split_treap(greater, t, &equal, &greater);
    root = merge_treap(logger, merge_treap(logger, less, equal), greater);
    free_request(t);
    return equal;
}

void erase_treap(Logger* logger, Treap** root, Request* tmp) {
    status_code st_act;
    Treap* less, *equal, *greater;
    split_treap(*root, tmp, &less, &greater);
    Request* t = NULL;
    st_act = request_copy(*tmp, &t);
    if (st_act != code_success) {
        free_request(t);
        *root = merge_treap(logger, less, greater);
        return;
    }
    strcat(t->id, "z");
    split_treap(greater, t, &equal, &greater);
    *root = merge_treap(logger, less, greater);
    free_request(t);
}

int treap_size(Treap* root) {
    if (!root) {
        return 0;
    } 
    return 1 + treap_size(root->left) + treap_size(root->right);
}

