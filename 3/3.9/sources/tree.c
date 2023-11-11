#include "../headers/tree.h"


status_cmd command(char** arg_one) {
    char* cmd = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!cmd) {
        return cmd_error_alloc;
    }
    int index = 0;
    char symbol = getchar();
    while (!isspace(symbol)) {
        cmd[index++] = symbol;
        symbol = getchar();
    }
    cmd[index] = '\0';
    index = 0;
    if (!strcmp(cmd, "Exit")) {
        free(cmd);
        return cmd_exit;
    }  else if (!strcmp(cmd, "Print")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        return cmd_print;
    } else if (!strcmp(cmd, "First")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        return cmd_first;
    } else if (!strcmp(cmd, "Len")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        return cmd_len;
    } else if (!strcmp(cmd, "Stats")) {
        free(cmd);
        return cmd_stat;
    } else if (!strcmp(cmd, "Find")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        return cmd_stat;
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}


bool valid_word(char letter, char* seps[], int size) {
    for (int i = 2; i < size; i++) {
        if (letter == seps[i][0]) {
            return false;
        }
    }
    return true;
}

status_code create_node(char* word, Node** node) {
    if (!word) return code_invalid_parameter;
    (*node) = malloc(sizeof(Node));
    if (!(*node)) {
        return code_error_alloc;
    }
    (*node)->count = 1;
    (*node)->left = NULL;
    (*node)->right = NULL;
    (*node)->word = strdup(word);
    return code_success;
}



status_code insert(Node** root, char* word) {
    if (!word) return code_invalid_parameter;
    status_code st_recursive;
    if (!(*root)) {
        st_recursive = create_node(word, root);
        return st_recursive;
    }
    if (strcmp((*root)->word, word) < 0) {
        st_recursive = insert(&((*root)->right), word); 
        return st_recursive;
    } else if (strcmp((*root)->word, word) > 0) {
        st_recursive = insert(&((*root)->left), word); 
        return st_recursive;
    } else {
        (*root)->count++;
    }
    return code_success;
}

Node* search_in_tree(Node *root, char *word) {
    if (root == NULL || strcmp(word, root->word) == 0) {
        return root;
    }
    if (strcasecmp(word, root->word) < 0) { 
        return search_in_tree(root->left, word);
    } else {
        return search_in_tree(root->right, word); 
    }
}

void free_tree(Node *root) {
    if (root) {
        free_tree(root->left);
        free_tree(root->right);
        free(root->word);
        root->word = NULL; 
        free(root);
    }
}

int compare_mets(Node* a, Node* b) {
    return a->count - b->count;
}

status_code push_list(List_tree** list, Node* add) {
    if (!add) return code_invalid_parameter;
    List_tree* new = malloc(sizeof(List_tree));
    new->element = add;
    if (!new) return code_error_alloc;
    if (!(*list) || compare_mets(new->element, (*list)->element)) {
        new->next = *list;
        *list = new;
        return code_success;
    }
    List_tree* current = *list;
    while (current->next != NULL && compare_mets(new->element, current->next->element) >= 0) {
        current = current->next;
    }

    new->next = current->next;
    current->next = new;

    return code_success;
}

status_code add_nodes_to_list(Node* root, List_tree** Tree_list) {
    if (!root || !Tree_list) return code_invalid_parameter;

    if (root->left != NULL) {
        status_code st_left = add_nodes_to_list(root->left, Tree_list);
        if (st_left != code_success) {
            return st_left;
        }
    }

    status_code st_push = push_list(Tree_list, root);
    if (st_push != code_success) {
        return st_push;
    }
    if (root->right != NULL) {
        status_code st_right = add_nodes_to_list(root->right, Tree_list);
        if (st_right != code_success) {
            return st_right;
        }
    }

    return code_success;
}

int size_list(List_tree* list) {
    if (!list) return 0;
    int count = 0;
    List_tree* tmp = list;
    while (!tmp) {
        count++;
        tmp = list->next;
    }
    return count;
}

void print_common_words(List_tree* list, int n) {
    if (n == 0) return;
    List_tree* current = list;
    int _size_list = size_list(list);
    int total_size = (_size_list < n) ? _size_list : n;
    for (int i = 0; i < total_size; i++) {
        int cur_size = current->element->count;
        printf("%s ", current->element->word);
        current = current->next;
    }
}

void print_max_len_word(List_tree* list) {
    if (!list) return;
    List_tree* current = list;
    char* max_word = NULL;
    int max = 0;
    int tmp = 0;
    while (current != NULL) {
        tmp = strlen(current->element->word);
        if (tmp > max) {
            max = tmp;
            max_word = current->element->word;
        }
        current = current->next;
    }
    printf("%s", max_word);
}

void print_least_len_word(List_tree* list) {
    if (!list) return;
    List_tree* current = list;
    char* less_word = current->element->word;
    int less = strlen(less_word);
    int tmp = 0;
    while (current != NULL) {
        tmp = strlen(current->element->word);
        if (tmp < less) {
            less = tmp;
            less_word = current->element->word;
        }
        current = current->next;
    }
    printf("%s", less_word);
}

void print_stats(Node *root) {
    if (root != NULL) {
        print_stats(root->left);
        printf("%s: %d\n", root->word, root->count);
        print_stats(root->right);
    }
}

int find_depth(Node* root) {
    if (root == NULL) {
        return 0;
    } else {
        int left_depth = find_depth(root->left);
        int right_depth = find_depth(root->right);
        return 1 + fmax(left_depth, right_depth);
    }
}


status_code read_from_file(char* argv[], int argc, Node** Tree, List_tree** Tree_list) {
    status_code st_act;
    FILE* in = fopen(argv[1], "r");
    if (!in) return code_error_oppening;
    char* line = NULL;
    int read;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        int size = strlen(line);
        int index = 0;
        bool scanned = false;
        char* word_to_add = malloc(sizeof(char) * STR_SIZE);
        bool start = true;
        for (int i = 0; i < size; i++) {
            char symbol = line[i];
            if (valid_word(symbol, argv, argc)) {
                start = false;
                scanned = true;
                word_to_add[index] = symbol;
                index++;
                printf("%c %d\n", word_to_add[index - 1], index);
            } else {
                if (scanned) {
                    word_to_add[index] = '\0';
                    index = 0;
                }
                if (!start && scanned) {
                    printf("%s 555\n", word_to_add);
                    st_act = insert(Tree, word_to_add);
                    if (st_act != code_success) {
                        free(word_to_add);
                        free(line);
                        fclose(in);
                        return st_act;
                    }
                    printf("%s 555\n", word_to_add);
                    free(word_to_add);
                    word_to_add = NULL;
                    word_to_add = malloc(sizeof(char) * STR_SIZE);
                    start = true;
                    scanned = false;

                }
            }
        }
        st_act = add_nodes_to_list(*Tree, Tree_list);
        if (st_act != code_success) {
            free(line);
            line = NULL;
            return st_act;
        }

        free(line);
        line = NULL;
    }
    if (line) free(line);
    fclose(in);
    return code_success;
}