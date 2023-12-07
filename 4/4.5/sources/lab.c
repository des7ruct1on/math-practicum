#include "../headers/lab.h"

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

status_code create_stack(Stack_char** stack) {
    (*stack) = (Stack_char*)malloc(sizeof(Stack_char));
    if (!(*stack)) return code_error_alloc;
    (*stack)->size = 0;
    (*stack)->top = NULL;
    return code_success;
}

bool is_empty_stack(Stack_char* st) {
    if (!st) return true;
    return st->size == 0;
}

int size_stack(Stack_char* st) {
    if (!st) return 0;
    return st->size;
}

char top_stack(Stack_char* st) {
    if (!st) return '\0';
    return st->top->data;
}

char pop_stack(Stack_char* st) {
    if (!st) return '\0';
    stack_node_char* tmp = st->top;
    char top_symb = tmp->data;
    st->top = st->top->next;
    free(tmp);
    st->size--;
    return top_symb;
}

status_code push_stack(Stack_char* st, char _data) {
    stack_node_char* tmp = (stack_node_char*)malloc(sizeof(stack_node_char));
    if (!tmp) return code_error_alloc;
    tmp->data = _data;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return code_success;
}

void free_stack(Stack_char* st) {
    if (!st) return;
    while (!is_empty_stack(st)) {
        pop_stack(st);
    }
    free(st);
}

bool is_operator(char c) {
    return (c == '+' || c == '*' || c == '/' || c == '^' || c == '-' || c == '%');
}

int get_priority(char c) {
    if (c == '*' || c == '/' || c == '%')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else if (c == '^')
        return 3;
    else
        return 0;
}

int op_assoc(char op) {
    if (op == '+' || op == '-' || op == '*' || op == '/' || op == '%')
        return 0;
    else
        return 1;
}

status_code to_pol_expression(char* expression, char** pol_expression) {
    //printf("\t%s\n", expression);
    if (!expression) return code_invalid_parameter;
    Stack_char* stack = NULL;
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
        char c = expression[i];
        //printf("symbol %c\n", c);
        if (isspace(c)) {
            continue;
        }
        if (is_operator(c)) {
            //printf("da ya operator\n");
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
                if ((op_assoc(c) == 0 && (get_priority(c) <= get_priority(s_top))) ||
                        (op_assoc(c) == 1 && (get_priority(c) < get_priority(s_top)))) {
                    (*pol_expression)[index] = s_top;
                    //printf("symbol %c\n", (*pol_expression)[index]);
                    index++;
                    
                    (*pol_expression)[index] = ',';
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
            
            if (!isdigit(expression[i + 1])) {
                (*pol_expression)[index] = ',';
                index++;
            }
            
        } else if (c == ')') {
            while(!is_empty_stack(stack) && top_stack(stack) != '(') {
                //printf("%c---dsadas\n", top_stack(stack));
                (*pol_expression)[index] = pop_stack(stack);
                index++;  
                if (is_operator((*pol_expression)[index - 1])) {
                    (*pol_expression)[index] = ',';
                    index++;  
                }
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
        if (is_operator((*pol_expression)[index - 1])) {
            (*pol_expression)[index] = ',';
            index++;  
        }
    }
    (*pol_expression)[index] = '\0';
    free_stack(stack);
    return code_success;
}

status_code create_stack_num(Stack_num** stack) {
    (*stack) = (Stack_num*)malloc(sizeof(Stack_num));
    if (!(*stack)) return code_error_alloc;
    (*stack)->size = 0;
    (*stack)->top = 0;
    return code_success;
}

bool is_empty_stack_num(Stack_num* st) {
    if (!st) return true;
    return st->size == 0;
}

int size_stack_num(Stack_num* st) {
    if (!st) return 0;
    return st->size;
}

long long top_stack_num(Stack_num* st) {
    if (!st) return 0;
    return st->top->data;
}

long long pop_stack_num(Stack_num* st) {
    if (!st) return '\0';
    stack_node_num* tmp = st->top;
    long long top_symb = tmp->data;
    st->top = st->top->next;
    free(tmp);
    st->size--;
    return top_symb;
}

status_code push_stack_num(Stack_num* st, long long _data) {
    stack_node_num* tmp = (stack_node_num*)malloc(sizeof(stack_node_num));
    if (!tmp) return code_error_alloc;
    tmp->data = _data;
    tmp->next = st->top;
    st->top = tmp;
    st->size++;
    return code_success;
}

void free_stack_num(Stack_num* st) {
    if (!st) return;
    while (!is_empty_stack_num(st)) {
        pop_stack_num(st);
    }
    free(st);
}


double binary_pow(double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = abs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return binary_pow(base, power - 1) * base;
    else {
        double tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

status_code compute(char* postfix, long long* res) {
    if (!postfix) return code_invalid_parameter;
    int size = strlen(postfix);
    Stack_num* s = NULL;
    status_code st_act;
    st_act = create_stack_num(&s);
    if (st_act != code_success) {
        free_stack_num(s);
        return st_act;
    }
    char* number = (char*)malloc(sizeof(char) * (size + 1));
    if (!number) {
        free_stack_num(s);
        free(number);
        return code_error_alloc;
    }
    int index = 0;
    long long a, b;
    int read;
    //printf("here\n");
    //printf("here\n");
    //printf("here\n");
    for (int i = 0; i < size; i++) {
        char c = postfix[i];
        if (isdigit(c)) {
            number[index] = c;
            index++;
            if (!isdigit(postfix[i + 1])) {
                number[index] = '\0';
                index = 0;
                read = sscanf(number, "%lld", &a);
                if (read != 1) {
                    free_stack_num(s);
                    free(number);
                    return code_invalid_parameter;
                }
                st_act = push_stack_num(s, a);
                if (st_act != code_success) {
                    free_stack_num(s);
                    free(number);
                    return st_act;
                }
                //printf("number %lld\n", a);
                free(number);
                number = NULL;
                number = (char*)malloc(sizeof(char) * (size + 1));
                if (!number) {
                    free_stack_num(s);
                    free(number);
                    return code_error_alloc;
                }
            }
        } else if (is_operator(c)) {
            a = pop_stack_num(s);
            if (is_empty_stack_num(s)) {
                free_stack_num(s);
                free(number);
                return code_invalid_parameter;
            }
            b = pop_stack_num(s);
            long long tmp;
            if (c == '+') {
                tmp = a + b;
            } else if (c == '-') {
                tmp = a - b;
            } if (c == '*') {
                tmp = a * b;
            } if (c == '^') {
                if ((a == 0 && b == 0) || b < 0) {
                    free_stack_num(s);
                    free(number);
                    return code_invalid_parameter;
                }
                //printf("\t a = %lld, b = %lld\n", a, b);
                tmp = binary_pow(b, a);
            } if (c == '/') {
                //printf("\t a = %lld, b = %lld\n", a, b);
                if (a == 0) {
                    //printf("here\n");
                    free_stack_num(s);
                    free(number);
                    //printf("here\n");
                    return code_invalid_parameter;
                }
                tmp = b / a;
            } if (c == '%') {
                if (a == 0) {
                    free_stack_num(s);
                    free(number);
                    return code_invalid_parameter;
                }
                tmp = b % a;
            }
            st_act =  push_stack_num(s, tmp);
            if (st_act != code_success) {
                free_stack_num(s);
                free(number);
                return st_act;
            }
            //printf("\ttmp = %lld\n", tmp);
        }
    }
    *res = pop_stack_num(s);
    free_stack_num(s);
    free(number);
    return code_success;
}
