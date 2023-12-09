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
    return (c == '~' || c == '?' || c == '!' || c == '+' || c == '&' || c == '|' || c == '-' || c == '<' || c == '=');
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
    } else {
        return 0;
    }
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

void* top_stack(Stack* st) {
    if (!st) return '\0';
    return st->top->data;
}

void* pop_stack(Stack* st) {
    if (!st) return '\0';
    stack_node* tmp = st->top;
    void* top_symb = tmp->data;
    st->top = st->top->next;
    free(tmp);
    st->size--;
    return top_symb;
}

status_code push_stack(Stack* st, void* _data) {
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


status_code to_pol_expression(char* expression, char** pol_expression) {
    //printf("\t%s\n", expression);
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
    printf("doshel\n");
    for (int i = 0; i < size_expression; i++) {
        char symbol = expression[i];
        printf("%c\n", symbol);
        if (isspace(symbol)) {
            continue;
        }
        if (isalpha(symbol) || symbol == '0' || symbol == '1') {
            (*pol_expression)[index] = symbol;
            index++;
        } else {
            if (symbol == ')') {
                while (true) {
                    void* ptr = NULL;
                    ptr = pop_stack(stack);
                    if (ptr == NULL) {
                        free_stack(stack);
                        free(*pol_expression);
                        return code_missed_left_bracket;
                    }
                    char st_char = *(char*)ptr;
                    if (st_char == '(') {
                        break;
                    }
                    (*pol_expression)[index] = st_char;
                    index++;
                }
            }
            else if (symbol == '('){
                st_act = push_stack(stack, &symbol);
                if (st_act != code_success) {
                    free_stack(stack);
                    free(*pol_expression);
                    return st_act;
                }
            }
            else {
                if (symbol == '+' || symbol == '-' || symbol == '<') {
                    if (expression[i + 1] != '>') {
                        free_stack(stack);
                        free(*pol_expression);
                        return code_invalid_parameter;
                    }
                }
                if (symbol == '>') {
                    if (expression[i - 1] != '+' && expression[i - 1] != '-' && expression[i - 1] != '<') {
                        free_stack(stack);
                        free(*pol_expression);
                        return code_invalid_parameter;
                    }
                    continue;
                }
                for (;;) {
                    if (is_empty_stack(stack)) {
                        break;
                    }
                    //printf("dasdas\n");
                    char s_top = *(char*)top_stack(stack);
                    //printf("pudasdasdasst\n");
                    if (!is_operator(s_top)) {
                        break;
                    }
                    //printf("    operator: %c\n", s_top);
                    if (((get_priority(symbol) <= get_priority(s_top))) ||
                            ((get_priority(symbol) < get_priority(s_top)))) {
                        (*pol_expression)[index] = s_top;
                        //printf("symbol %c\n", (*pol_expression)[index]);
                        //index++;
                        
                        //(*pol_expression)[index] = ',';
                        //printf("symbol %c\n", (*pol_expression)[index]);
                        index++;
                        
                        void* tmp = pop_stack(stack);
                    } else {
                        break;
                    }
                }
                st_act = push_stack(stack, &symbol);
                if (st_act != code_success) {
                    free_stack(stack);
                    free(*pol_expression);
                    *pol_expression = NULL;
                    return st_act;
                }

            }
        }

    }
    void *ptr = NULL;
    while (!is_empty_stack(stack)) {
        ptr = pop_stack(stack);
        char from_stack = *(char *)ptr;
        if (from_stack == '(') {
            free_stack(stack);
            free((*pol_expression));
            return code_missed_right_bracket;
        }
        (*pol_expression)[index] = from_stack;
        index++;
    }
    (*pol_expression)[index] = '\0';
    free_stack(stack);
    return code_success;
}

status_code create_ariph_tree(Tree** tree, char* postfix) {
    if (!postfix) return code_invalid_parameter;
    Stack* stack = NULL;
    status_code st_act;

    int size = strlen(postfix);

    for (int i = 0; i < size; i++) {
        if (isalpha(postfix[i]) || postfix[i] == '0' || postfix[i] == '1') {
            Tree* node = NULL;
            st_act = create_tree(&node, postfix[i]);
            if (st_act != code_success) {
                free(postfix);
                free_stack(stack);
                return st_act;
            }
            st_act = push_stack(stack, node);
            if (st_act != code_success) {
                free(postfix);
                free_stack(stack);
                return st_act;
            }
        } else {
            void* ptr = NULL;
            ptr = pop_stack(stack);
            if (ptr == NULL) {
                free(postfix);
                free_stack(stack);
                return code_invalid_parameter;
            }
            Tree* op_left = (Tree*)ptr;
            Tree* res = NULL;
            if (postfix[i] == '~') {
                st_act = create_tree(&res, postfix[i]);
                if (st_act != code_success) {
                    free(postfix);
                    free_stack(stack);
                    return st_act;
                }
                res->left = op_left;
            } 
            else {
                void* ptr_2 = NULL;
                ptr_2 = pop_stack(stack);
                if (ptr_2 == NULL) {
                    free(postfix);
                    free_stack(stack);
                    return code_invalid_parameter;
                }
                Tree* op_right = (Tree*)ptr_2;
                st_act = create_tree(&res, postfix[i]);
                if (st_act != code_success) {
                    free(postfix);
                    free_stack(stack);
                    return st_act;
                }
                res->left = op_left;
                res->right = op_right;
            }
            st_act = push_stack(stack, res);
            if (st_act != code_success) {
                free(postfix);
                free_stack(stack);
                free_tree(res);
                return st_act;
            }
        }
    }
    void* ptr = NULL;
    ptr = pop_stack(stack);
    (*tree) = (Tree*)ptr;
    if (stack) {
        free_stack(stack);
        free(postfix);
        return code_invalid_parameter;
    }
    free(postfix);
    return code_success;
}