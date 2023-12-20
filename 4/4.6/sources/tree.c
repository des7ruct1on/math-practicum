#include "../headers/tree.h"


status_code create_tree(Tree** tree, char value) {
    (*tree) = (Tree*)malloc(sizeof(Tree));
    if (!(*tree)) return code_error_alloc;
    (*tree)->left = NULL;
    (*tree)->right = NULL;
    (*tree)->data = value;
    return code_success;
}

status_code insert_tree(Tree** tree, char value) {
    if (isspace(value) || value == '\r') return code_invalid_parameter;
    status_code st_rec;
    if (!(*tree)) {
        return create_tree(tree, value);
    }
    if ((*tree)->data < value) {
        st_rec = insert_tree(&((*tree)->left), value); 
        return st_rec;
    } else if ((*tree)->data > value) {
        st_rec = insert_tree(&((*tree)->right), value); 
        return st_rec;
    } 
    return code_success;
}

void free_tree(Tree* tree) {
    if (!tree) return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree);
    tree = NULL;
}

bool is_operator(char c) {
    return (c == '~' || c == '?' || c == '!' || c == '+' || c == '&' || c == '|' || c == '-' || c == '<' || c == '>' || c == '=');
}

int get_priority(char c) {
    if (c == '~') {
        return 3;
    }
    if (c == '?' || c == '!' || c == '+' || c == '&') {
        return 2;
    }
    if (c == '|' || c == '-' || c == '<' || c == '=') {
        return 1;
    } 
    return 0;
}

/*
int op_assoc(char op) {
    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%')
        return 0;
    else
        return 1;
}
*/




status_code create_stack(Stack** stack) {
    (*stack) = (Stack*)malloc(sizeof(Stack));
    if (!(*stack)) return code_error_alloc;
    (*stack)->size = 0;
    (*stack)->top = NULL;
    return code_success;
}

bool is_empty_stack(Stack* st) {
    if (!st) return true;
    return st->size == 0;
}

int size_stack(Stack* st) {
    if (!st) return 0;
    return st->size;
}

char top_stack(Stack* st) {
    if (!st) return '\0';
    return st->top->data;
}

char pop_stack(Stack* st) {
    if (!st) return '\0';
    stack_node* tmp = st->top;
    char top_symb = tmp->data;
    st->top = st->top->next;
    free(tmp);
    st->size--;
    return top_symb;
}

status_code push_stack(Stack* st, char _data) {
    stack_node* tmp = (stack_node*)malloc(sizeof(stack_node));
    if (!tmp) return code_error_alloc;
    tmp->data = _data;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return code_success;
}

void free_stack(Stack* st) {
    if (!st) return;
    while (!is_empty_stack(st)) {
        pop_stack(st);
    }
    free(st);
}

void print_stack(Stack* st) {
    if (!st->size) return;
    stack_node * cur = st->top;
    printf("size = %d\n", st->size);
    while (cur != NULL) {
        printf("%c_", cur->data);
        cur = cur->next;
    }
    printf("\n");
}



status_code create_stack_tree(Stack_tree** stack) {
    (*stack) = (Stack_tree*)malloc(sizeof(Stack_tree));
    if (!(*stack)) return code_error_alloc;
    (*stack)->size = 0;
    (*stack)->top = NULL;
    return code_success;
}

bool is_empty_stack_tree(Stack_tree* st) {
    if (!st) return true;
    return st->size == 0;
}

int size_stack_treee(Stack_tree* st) {
    if (!st) return 0;
    return st->size;
}

Tree* top_stack_tree(Stack_tree* st) {
    if (!st) return NULL;
    return st->top->data;
}

Tree* pop_stack_tree(Stack_tree* st) {
    if (!st) return NULL;
    stack_node_tree* tmp = st->top;
    Tree* top_symb = tmp->data;
    st->top = st->top->next;
    free(tmp);
    st->size--;
    return top_symb;
}

status_code push_stack_tree(Stack_tree* st, Tree* _data) {
    stack_node_tree* tmp = (stack_node_tree*)malloc(sizeof(stack_node_tree));
    if (!tmp) return code_error_alloc;
    tmp->data = _data;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return code_success;
}

void free_stack_tree(Stack_tree* st) {
    if (!st) return;
    while (!is_empty_stack_tree(st)) {
        pop_stack_tree(st);
    }
    free(st);
}


status_code to_pol_expression(char* expression, char** pol_expression) {
    printf("\t%s\n", expression);
    if (!expression) return code_invalid_parameter;
    Stack* stack = NULL;
    status_code st_act;
    st_act = create_stack(&stack);
    if (st_act != code_success) return st_act;
    int size_expression = strlen(expression);
    (*pol_expression) = (char*)malloc(sizeof(char) * (size_expression * 2));
    if (!(*pol_expression)) {
        free_stack(stack);
        return code_error_alloc;
    }
    int index = 0;
    //printf("doshel\n");
    for (int i = 0; i < size_expression; i++) {
        //print_stack(stack);
        char c = expression[i];
        //printf("symbol %c\n", c);
        if (isspace(c)) {
            continue;
        }
        if (is_operator(c)) {
            //printf("da ya operator\n");
            if (c == '+' || c == '-' || c == '<') {
                if (expression[i + 1] != '>') {
                    free_stack(stack);
                    free(*pol_expression);
                    *pol_expression = NULL;
                    //printf("here ban1\n");
                    return code_invalid_parameter;
                }
            }
            if (c == '>') {
                if (expression[i - 1] != '+' && expression[i - 1] != '-' && expression[i - 1] != '<') {
                    free_stack(stack);
                    free(*pol_expression);
                    *pol_expression = NULL;
                    //printf("here ban2\n");
                    return code_invalid_parameter;
                }
                continue;
            }
            for (;;) {
                if (is_empty_stack(stack)) {
                    //printf("pust\n");
                    break;
                }
                //printf("dasdas\n");
                char s_top = top_stack(stack);
                //printf("pudasdasdasst\n");
                if (!is_operator(s_top)) {
                    break;
                }
                //printf("    operator: %c\n", s_top);
                if (get_priority(c) <= get_priority(s_top)) {
                    (*pol_expression)[index] = s_top;
                    //printf("symbol %c\n", (*pol_expression)[index]);
                    index++;
                    
                    pop_stack(stack);
                } else {
                    break;
                }
            }
            st_act = push_stack(stack, c);
            if (st_act != code_success) {
                free_stack(stack);
                free(*pol_expression);
                *pol_expression = NULL;
                return st_act;
            }
        } else if (isalpha(c) || isdigit(c)) {
            (*pol_expression)[index] = c;
            index++;
        } else if (c == ')') {
           // print_stack(stack);
            while(!is_empty_stack(stack) && top_stack(stack) != '(') {
                //printf("%c---dsadas\n", top_stack(stack));
                (*pol_expression)[index] = pop_stack(stack);
                index++;  
            }
            if (is_empty_stack(stack)) {
                //printf("bsfdggsdfgfs\n");
                free_stack(stack);
                free(*pol_expression);
                (*pol_expression) = NULL;
                return code_missed_left_bracket;
            }
            pop_stack(stack);
        } else if (c == '(') {
            st_act = push_stack(stack, c);
            if (st_act != code_success) {
                free_stack(stack);
                free(*pol_expression);
                *pol_expression = NULL;
                return st_act;
            }
            //printf("zapushil, %d\n", size_stack(stack));
        } else if (c == '\0') {
            break;
        } else {
            free_stack(stack);
            free(*pol_expression);
            *pol_expression = NULL;
            return code_invalid_parameter;
        }
        //printf("\t\t index %d\n", index);
    }
    while (!is_empty_stack(stack)) {
        if (top_stack(stack) == '(') {
            free_stack(stack);
            free(*pol_expression);
            *pol_expression = NULL;
            return code_missed_right_bracket;
        }
        (*pol_expression)[index] = pop_stack(stack);
        //printf("----%c\n", (*pol_expression)[index]);
        index++;  
    }
    (*pol_expression)[index] = '\0';
    free_stack(stack);
    return code_success;
}

void print_tree(Tree* root, int level) {
    if (!root) return;
    print_tree(root->left, level + 1);
    for (int i = 0; i < level; ++i) printf("  ");
    printf("%c\n", root->data);
    print_tree(root->right, level + 1);
}

status_code create_arifmh_tree(Tree** tree, char* postfix) {
    if (!postfix) return code_invalid_parameter;
    Stack_tree* stack = NULL;
    status_code st_act;
    st_act = create_stack_tree(&stack);
    if (st_act != code_success) {
        free(postfix);
        return code_error_alloc;
    }
    int size = strlen(postfix);

    for (int i = 0; i < size; i++) {
        if (isalpha(postfix[i]) || postfix[i] == '0' || postfix[i] == '1') {
            Tree* node = NULL;
            st_act = create_tree(&node, postfix[i]);
            if (st_act != code_success) {
                free(postfix);
                free_stack_tree(stack);
                return st_act;
            }
            //printf("suka\n");
            st_act = push_stack_tree(stack, node);
            if (st_act != code_success) {
                free(postfix);
                free_stack_tree(stack);
                return st_act;
            }
            //printf("suka2\n");
        } else {
            Tree* op_left = pop_stack_tree(stack);
            //print_tree(op_left, 0);
            Tree* res = NULL;
            if (postfix[i] == '~') {
                st_act = create_tree(&res, postfix[i]);
                if (st_act != code_success) {
                    free(postfix);
                    free_stack_tree(stack);
                    return st_act;
                }
                res->left = op_left;
            } 
            else {
                Tree* op_right = pop_stack_tree(stack);
                st_act = create_tree(&res, postfix[i]);
                if (st_act != code_success) {
                    free(postfix);
                    free_stack_tree(stack);
                    return st_act;
                }
                res->left = op_left;
                res->right = op_right;
            }
            st_act = push_stack_tree(stack, res);
            if (st_act != code_success) {
                free(postfix);
                free_stack_tree(stack);
                free_tree(res);
                return st_act;
            }
        }
    }
    Tree* total = pop_stack_tree(stack);
    (*tree) = total;
    if (stack->size != 0) {
        free_stack_tree(stack);
        free(postfix);
        return code_invalid_parameter;
    }
    free_stack_tree(stack);
    free(postfix);
    //print_tree(*tree, 0);
    return code_success;
}

int compare(const char *first, const char *second) {
    return *first - *second;
}

void print_vars(FILE *stream, char *vars, int count, char *infix) {
    for (int i = 0; i < count; i++) {
        fprintf(stream, " %c |", vars[i]);
    }
    fprintf(stream, " %s\n", infix);
    fprintf(stream, "________________________________");
}

void print_table(FILE *stream, int *values, const int count, int result) {
    fprintf(stream, "\n");
    for (int i = 0; i < count; i++) {
        fprintf(stream, " %d |", values[i]);
    }
    fprintf(stream, " %d\n", result);
    fprintf(stream, "________________________________");
}

status_code get_vars(char** vars, char* infix) {
    size_t size = 0;
    size_t capacity = 2;
    (*vars) = (char*)malloc(sizeof(char) * capacity);
    if (!(*vars)) return code_error_alloc;
    int infix_size = strlen(infix);
    for (int i = 0; i < infix_size; i++) {
        if (!isalpha(infix[i])) {
            continue;
        }
        bool is_exist = false;
        for (int j = 0; j < size; j++) {
            if (infix[i] == (*vars)[j]) {
                is_exist = true;
                break;
            }
        }
        if (is_exist) {
            continue;
        }
        (*vars)[size] = infix[i];
        size++;
        if (size >= capacity) {
            capacity *= 2;
            char *tmp = NULL;
            if (!(tmp = (char *)realloc((*vars), sizeof(char) * capacity))) {
                free(*vars);
                return code_error_alloc;
            }
            (*vars) = tmp;
        }
    }
    (*vars)[size] = '\0';
    qsort((*vars), size, sizeof(char), (int(*)(const void *, const void *))compare);
    return code_success;
}


int compute_tree(Tree* root, char* vars, int* values, int count_vars) {
    if (!root) return 0;

    int left = compute_tree(root->left, vars, values, count_vars);
    int right = compute_tree(root->right, vars, values, count_vars);
    if (isalpha(root->data)) {
        for (int i = 0; i < count_vars; i++) {
            if (root->data == vars[i]) {
                return values[i];
            }
        }
    }
    if (root->data == '0' || root->data == '1') {
        return root->data - '0';
    }
    switch (root->data) {
        case '&':
            return left & right;
        case '|':
            return left | right;
        case '~':
            return !left;
        case '-':
            return left < right;
        case '+':
            return left >= right;
        case '<':
            return left != right;
        case '=':
            return left == right;
        case '!':
            return !(left & right);
        case '?':
            return !(left | right);
    }
    return 0;
}

void get_rand_filename(char *filename) {
    int size = rand() % 6 + 1;
    char result[size];
    for (int i = 0; i < size;) {
        result[i] = rand() % ('Z' - '0' + 1) + '0';
        if (isalpha(result[i]) || isdigit(result[i])) {
            i++;
        }
    }
    result[size] = '\0';
    strcat(result, ".txt");
    strcpy(filename, result);
}

void print_error(status_code st) {
    switch (st) {
        case code_invalid_parameter:
            fprintf(stdout, "Invalid parameter detected!!!\n");
            break;
        case code_error_alloc:
            fprintf(stdout, "Error alloc detected!!!\n");
            break;
        case code_error_oppening:
            fprintf(stdout, "Can`t open file!!!\n");
            break;
        case code_missed_left_bracket:
            fprintf(stdout, "You missed left bracket!!!\n");
            break;
        case code_missed_right_bracket:
            fprintf(stdout, "You missed right bracket!!!\n");
            break;
        default:
            break;
    }
}

status_code truth_table(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        return code_error_oppening;
    }
    char* line = NULL;
    status_code st_act;
    getline(&line, &(size_t){0}, input);
    int string_size = strlen(line);
    if (!string_size || !line || line[string_size - 1] == '\n') {
        fclose(input);
        return code_invalid_parameter;
    }
    fclose(input);
#ifdef DEBUG
    printf("jere\n");
#endif
    Tree *root = NULL;
    char* pol_expression = NULL;
    st_act = to_pol_expression(line, &pol_expression);
    if (st_act != code_success) {
        free(pol_expression);
        pol_expression = NULL;
        return st_act;
    }
    //printf("%s--\n", pol_expression);
    st_act = create_arifmh_tree(&root, pol_expression);
    if (st_act != code_success) {
        free_tree(root);
        free(line);
        return st_act;
    }
    //printf("ushze\n");
    char out_filename[128];
    get_rand_filename(out_filename);
    FILE *out = fopen(out_filename, "w");
    if (!out) {
        free_tree(root);
        free(line);
        return code_error_oppening;
    }
    char *vars = NULL;
    st_act = get_vars(&vars, line);
    if (st_act != code_success) {
        free(line);
        free_tree(root);
        return st_act;
    }
    int count_vars = strlen(vars);
    long long int combinations_count = 1 << count_vars; // количество комбинаций
    int vars_values[count_vars];
    print_vars(out, vars, count_vars, line);
    for (long long int i = 0; i < combinations_count; i++) {
        for (int j = 0; j < count_vars; j++) {
            vars_values[j] = (i >> j) & 1;
        }
        int comp_res = compute_tree(root, vars, vars_values, count_vars);
        //printf("this %d\n", comp_res);
        print_table(out, vars_values, count_vars, comp_res);
    }
    free(line);
    free_tree(root);
    fclose(out);
    free(vars);
    return code_success;

}