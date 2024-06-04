#include "headers/bst.h"

status_code create_tree(Tree** tree, char* key, Post* office) {
    if (!office) return code_invalid_parameter;
    (*tree) = (Tree*)malloc(sizeof(Tree));
    if (!*tree) return code_error_alloc;
    (*tree)->key = key;
    (*tree)->post = office;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    return code_success;
}

status_code insert_tree(Logger* logger, Tree** tree, char* key, Post* office) {
    if (!key || !office) return code_invalid_parameter;
    status_code st_act;
    if (!*tree) {
        st_act = create_tree(tree, key, office);
        if (st_act != code_success) {
            create_log(&logger, "error after creating tree, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
        }
        return st_act;
    }
    if (strcmp(key, (*tree)->key) < 0) {
        st_act = insert_tree(logger, &(*tree)->left, key, office);
        if (st_act != code_success) {
            create_log(&logger, "error after inserting tree, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
        }
    } else if (strcmp(key, (*tree)->key) > 0) {
        st_act = insert_tree(logger, &(*tree)->right, key, office);
        if (st_act != code_success) {
            create_log(&logger, "error after inserting tree, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
        }
    }
    return code_success;
}

void free_tree(Tree* root, void(*free_storage)(void*)) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left, free_storage);
    free_tree(root->right, free_storage);
    if (root->key) free(root->key);
    free_storage(root->post->storage);
    free(root->post->ops);
    free(root);
}

Post* search_bst(Tree* root, char* key) {
    if (!root || !key) return NULL; 

    int cmp = strcmp(key, root->key);
    if (cmp < 0) {
        return search_bst(root->left, key);
    } else if (cmp > 0) {
        return search_bst(root->right, key);
    } else {
        return root->post;
    }
}

int tree_size(Tree* root) {
    if (!root) {
        return 0;
    } 
    return 1 + tree_size(root->left) + tree_size(root->right);
}

char* find_post_tree(Tree* root, Post* find) {
    if (root == NULL) {
        return NULL; 
    }

    if (root->post == find) {
        return root->key; 
    } else {
        char* result_left = find_post_tree(root->left, find);
        if (result_left != NULL) {
            return result_left; 
        }
        
        char* result_right = find_post_tree(root->right, find);
        if (result_right != NULL) {
            return result_right; 
        }
    }
    
    return NULL; 
}
