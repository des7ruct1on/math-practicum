#include "headers/treap.h"

Treap* merge_treap(Treap* a, Treap* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->data.priority > b->data.priority) {
        a->right = merge_treap(a->right, b);
        return a;
    } else {
        b->left = merge_treap(a, b->left);
        return b;
    }
}

status_code treap_copy(Treap** copy, Treap* add) {
    if (!add) {
        *copy = NULL;
        return code_success;
    }
    *copy = (Treap*)malloc(sizeof(Treap));
    if (!*copy) return code_error_alloc;

    status_code st_act;
    st_act = request_copy(add->data, &(*copy)->data);
    if (st_act != code_success) {
        free_treap(*copy);
    }
    st_act = treap_copy(&(*copy)->left, add->left);
    if (st_act != code_success) {
        free_treap(*copy);
    }
    st_act = treap_copy(&(*copy)->right, add->right);
    if (st_act != code_success) {
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

Treap* merge_treap_copy(Treap* a, Treap* b) {
    if (!a && !b) return NULL;
    Treap* lc = NULL;
    Treap* rc = NULL;
    status_code st_act;
    st_act = treap_copy(&lc, a);
    if (st_act != code_success) {
        return NULL;
    }
    st_act = treap_copy(&rc, b);
    if (st_act != code_success) {
        free_treap(a);
        return NULL;
    }
    return merge_treap(lc, rc);
}

void split_treap(Treap* root, Request* key, Treap** a, Treap** b) {
    if (!root) {
        *a = *b = NULL;
        return;
    }
    if (strcmp(root->data.id, key->id) < 0) {
        split_treap(root->right, key->id, &root->right, b);
        *a = root;
        return;
    } else {
        split_treap(root->left, key->id, a, &root->left);
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

void insert_treap(Treap** root, Request tmp) {
    Treap* less, *greater;
    split_treap(*root, (*root)->data.id, &less, &greater);
    Treap* add = create_node_treap(tmp);
    *root = merge_treap(merge_treap(less, add), greater);
}

Treap* search_treap(Treap* root, Request* tmp) {
    Treap* less, *equal, *greater;
    split_treap(root, tmp->id, &less, &greater);
    split_treap(greater, tmp->id + 1, &equal, &greater);
    root = merge_treap(merge_treap(less, equal), greater);
    return equal;
}

void erase_treap(Treap** root, Request* tmp) {
    Treap* less, *equal, *greater;
    split_treap(*root, tmp->id, &less, &greater);
    split_treap(greater, tmp->id + 1, &equal, &greater);
    *root = merge_treap(less, greater);
}

void print_treap(Treap* root, int n) {
    if (root == NULL) {
        return;
    }
    print_treap(root->right, n + 1);
    for (int i = 0; i < n; i++) {
        printf("\t");
    }
    printf("<%d, %d>\n", root->data.id, root->data.priority);
    print_treap(root->left, n + 1);
}

int treap_size(Treap* root) {
    if (!root) {
        return 0;
    } 
    return 1 + tree_size(root->left) + tree_size(root->right);
}

