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
    } else if (!strcmp(cmd, "Depth")) {
        free(cmd);
        return cmd_depth;
    } else if (!strcmp(cmd, "Print")) {
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
    } else if (!strcmp(cmd, "Get")) {
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
        return cmd_get_file;
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
        return cmd_find;
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}


bool valid_char(char letter, char* seps[], int size) {
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

status_code get_tree_from_file(FILE* file, Node** root, List_tree** Tree_list) {
    status_code st_act;
    char* line = NULL;
    if (*root) {
        free_tree(*root);
        *root = NULL;
    }
    while (getline(&line, &(size_t){0}, file) != -1) {
        int count = 0;
        char word[STR_SIZE];
        sscanf(line, "%d%s", &count, word);
        printf("%d -- %s\n", count, word);
        for (int i = 0; i < count; i++) {
            st_act = insert(root, word);
            if (st_act != code_success) {
                free(line);
                return st_act;
            }
            free(line);
            line = NULL;
        }
    }
    free(line);
    line = NULL;
    st_act = add_nodes_to_list(*root, Tree_list);
    return st_act;
}

status_code insert(Node** root, char* word) {
    if (!strcmp(word, "\n")) return code_success;
    if (!word) return code_invalid_parameter;
    status_code st_recursive;
    if (!(*root)) {
        st_recursive = create_node(word, root);
        return st_recursive;
    }
    if (strcmp((*root)->word, word) < 0) {
        st_recursive = insert(&((*root)->left), word); 
        return st_recursive;
    } else if (strcmp((*root)->word, word) > 0) {
        st_recursive = insert(&((*root)->right), word); 
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
    if (strcmp(word, root->word) < 0) { 
        return search_in_tree(root->right, word);
    } else {
        return search_in_tree(root->left, word); 
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
    List_tree* new = (List_tree*)malloc(sizeof(List_tree));
    if (!new) return code_error_alloc;
    new->element = add;
    if (!(*list)) {
        *list = new;
        new->next = NULL;
    } else {
        if (!(*list)->next) {
            if (compare_mets((*list)->element, add)) {
                (*list)->next = new;
                new->next = NULL;
            } else {
                List_tree* tmp = *list;
                *list = new;
                new->next = tmp;
            }
        } else {
            List_tree* current = *list;
            int check = compare_mets(current->element, new->element);
            if (check <= 0) {
                new->next = current;
                *list = new;
                return code_success;
            }
            while (current->next != NULL) {
                check = compare_mets(current->next->element, new->element);
                if (check < 0) {
                    break;
                } 
                current = current->next;
            }
            List_tree* new_next = current->next;
            current->next = new;
            new->next = new_next;
        }
    }

    return code_success;
}

status_code add_nodes_to_list(Node* root, List_tree** Tree_list) {
    if (!root || !Tree_list) return code_invalid_parameter;

    status_code st_push = push_list(Tree_list, root);
    if (st_push != code_success) {
        return st_push;
    }

    if (root->left != NULL) {
        status_code st_left = add_nodes_to_list(root->left, Tree_list);
        if (st_left != code_success) {
            return st_left;
        }
    }
    if (root->right != NULL) {
        status_code st_right = add_nodes_to_list(root->right, Tree_list);
        if (st_right != code_success) {
            return st_right;
        }
    }

    return code_success;
}

void print_list(List_tree* list) {
    if (!list) return;
    List_tree* cur = list;
    while (cur != NULL) {
        printf("%s\t", cur->element->word);
        cur = cur->next;
    }
}

int size_list(List_tree* list) {
    if (!list) return 0;
    int count = 0;
    List_tree* tmp = list;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
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

int get_size_tree(Node* tree) {
    if (!tree) return 0;
    return 1 + get_size_tree(tree->left) + get_size_tree(tree->right);
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

void write_to_file(FILE* file, Node* tree, int depth) {
    if (!tree) return;
    for (int i = 0; i < depth; i++) fprintf(file, "\t");
    fprintf(file, "%d %s\n", tree->count, tree->word);
    write_to_file(file, tree->left, depth + 1);
    write_to_file(file, tree->right, depth + 1);
}

status_code read_from_file(char* argv[], int argc, Node** Tree, List_tree** Tree_list) {
    status_code st_act;
    FILE* in = fopen(argv[1], "r");
    if (!in) return code_error_oppening;
    char* word = (char*)malloc(sizeof(char) & STR_SIZE);
    if (!word) {
        fclose(in);
        return code_error_alloc;
    }
    char symbol = fgetc(in);
    int index_word = 0;
    while (symbol != EOF) {
        if (valid_char(symbol, argv, argc)) {
            word[index_word] = symbol;
            index_word++;
            //printf("\t%c---%d\n", symbol, index_word);
        } else {
            if (index_word != 0) {
                word[index_word] = '\0';
                //printf("\t%s %d\n", word, index_word);
                index_word = 0;
                st_act = insert(Tree, word);
                if (st_act != code_success) {
                    free(word);
                    fclose(in);
                    return st_act;
                }
                free(word);
                word = NULL;
                word = (char*)malloc(sizeof(char) & STR_SIZE);
                if (!word) {
                    fclose(in);
                    return code_error_alloc;
                }
            } 
        }  
        //printf("\t%c---%d\n", symbol, index_word);
        symbol = fgetc(in);
    }
    if (word) {
        free(word);
        word = NULL;
    }
    st_act = add_nodes_to_list(*Tree, Tree_list);
    fclose(in);
    return st_act;
}

void destroy_list(List_tree* list) {
    if (!list) return;
    List_tree* tmp = list;
    while (tmp->next != NULL) {
        List_tree* del = tmp->next;
        free(tmp);
        tmp = NULL;
        tmp = del;
    }
    free(tmp);
}