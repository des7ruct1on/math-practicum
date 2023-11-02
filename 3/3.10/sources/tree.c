#include "../headers/tree.h"

void delete_tree(Node* node) {
    if (!node) {
        return;
    }
    delete_tree(node->child_main);
    delete_tree(node->right_child);
    free(node);
}

status_code insert_tree_main(Node* node, char data) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (!new) {
        return code_error_alloc;
    }
    new->key = data;
    new->parent = node;
    new->left_child = NULL;
    new->right_child = NULL;
    node->child_main = new;
    return code_success;
}

status_code insert_tree_right(Node* node, char data) {
    Node* new = (Node*)malloc(sizeof(Node));
    if (!new) {
        return code_error_alloc;
    }
    new->key = data;
    new->parent = node->parent;
    new->left_child = node;
    new->right_child = NULL;
    new->child_main = NULL;
    node->right_child = new;
    return code_success;
}

status_code build_tree(Tree* tree, char* expression) {
    int length = strlen(expression);
    char symb;
    Node* _root = (Node*)malloc(sizeof(Node));
    if (!_root) {
        return code_error_alloc;
    }
    int i = 0;
    _root->key = expression[i];
    _root->child_main = NULL;
    _root->left_child = NULL;
    _root->right_child = NULL;
    _root->parent = NULL;
    tree->root = _root;
    i++;
    Node* current = tree->root;
    status_code st_insert;
    for (i; i < length; i++) {
        symb = expression[i];
        switch (symb) {
            case '(':
                i++;
                st_insert = insert_tree_main(current, expression[i]);
                if (st_insert == code_error_alloc) {
                    return code_error_alloc;
                }
                current = current->child_main;
                break;
            case ')':
                current = current->parent;
                break;
            case ',':
                i++;
                st_insert = insert_tree_right(current, expression[i]);
                if (st_insert == code_error_alloc) {
                    return code_error_alloc;
                }
                current = current->right_child;
                break;
            default:
                break;
        }
    }
    return code_success;
}

void print_node(FILE* stream, Node* node, int depth) {
    if (!node) {
        return;
    }
    if (node->right_child) {
        print_node(stream, node->right_child, depth);
    }
    if (node->child_main) {
        print_node(stream, node->child_main, depth + 1);
    }
    for (int i = 0; i < depth; i++) {
        fprintf(stream, "   ");
    }

    if (node->right_child) {
        fprintf(stream, "%c    \n", node->key);
    } else {
        fprintf(stream, "%c    \n", node->key);
    }
}

void print_tree(FILE* stream, Tree* tree, char* expression) {
    int depth = 0;
    fprintf(stream, "Expression: %s\n", expression);
    print_node(stream, tree->root, depth);
    fprintf(stream, "\n");
}

status_code scan_print_tree(FILE* input, FILE* output) {
    if (!input || !output) {
        return code_invalid_parameter;
    }
    Tree _root;
    _root.root = NULL;
    status_code st_build;
    char* expression = NULL;
    int read;
    while ((read = getline(&expression, &(size_t){0}, input)) != -1) {
        if (read == -1) {
            free(expression);
            return code_error_alloc;
        }
        st_build = build_tree(&_root, expression);
        if (st_build == code_error_alloc) {
            free(expression);
            return code_error_alloc;
        }
        print_tree(output, &_root, expression);
        free(expression);
        expression = NULL;
        delete_tree(_root.root);
        _root.root = NULL;
    }
    free(expression);
    return code_success;
}