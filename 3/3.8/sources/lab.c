#include "../headers/lab.h"

int compare_monoms(Polynom* a, Polynom* b) {
    return  a->degree - b->degree;
}

status_code push_monom(Polynom** equation, int _degree, int _coef) {
    if (_degree == 0 && _coef == 0) return code_invalid_parameter;
    Polynom* new = (Polynom*)malloc(sizeof(Polynom));
    if (!new) return code_error_alloc;
    new->coef = _coef;
    new->degree = _degree;
    if (!(*equation)) {
        *equation = new;
        new->next = NULL;
    } else {
        if (!(*equation)->next) {
            if (compare_monoms((*equation), new) < 0) {
                (*equation)->next = new;
                new->next = NULL;
            } else {
                Polynom* tmp = *equation;
                *equation = new;
                new->next = tmp;
            }
        } else {
            Polynom* current = *equation;
            int check = compare_monoms(current, new);
            if (check >= 0) {
                new->next = current;
                *equation = new;
                return code_success;
            }
            while (current->next != NULL) {
                check = compare_monoms(current->next, new);
                if (check > 0) {
                    break;
                } 
                current = current->next;
            }
            Polynom* new_next = current->next;
            current->next = new;
            new->next = new_next;
        }
    }

    return code_success;
}

int size_list(Polynom* list) {
    if (!list) return 0;
    int count = 0;
    Polynom* tmp = list;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

void print_polynom(Polynom* equation) {
    if (!equation) return;
    Polynom* current = equation;
    printf("size polynom: %d\n", size_list(equation));
    bool start = true;
    while (current != NULL) {
        if (!start) {
            if (current->coef > 0) {
                printf("+ ");
            }
        }
        if (current->coef != 1 && current->coef != -1) printf("%d", current->coef);
        if (current->coef == -1) {
            if (current->degree != 0) {
                printf("-");
            } else {
                printf("%d", current->coef);
            }
        }
        if (current->degree != 0) {
            if (current->degree > 9) {
                printf("x^(%d) ", current->degree);
            } else if (current->degree > 1) {
                printf("x^%d ", current->degree);
            } else {
                printf("x ");
            }
        } else {
            printf(" ");
        }
        current = current->next;
        start = false;
    }
    printf("\n");
}

void destroy_polynomial(Polynom* list) {
    if (!list) return;
    Polynom* tmp = list;
    while (tmp->next != NULL) {
        Polynom* del = tmp->next;
        free(tmp);
        tmp = NULL;
        tmp = del;
    }
    free(tmp);
}

status_code read_from_file(const char* filename) {
    FILE* in = fopen(filename, "r");
    if (!in) return code_error_oppening;
    char* line = NULL;
    Polynom* summator = NULL;
    bool is_long_comment = false;
    int read;
    int index = 0;
    char symbol;
    status_code st_act;
    char* cmd = (char*)malloc(sizeof(char) * MAX_SIZE);
    if (!cmd) {
        return code_error_alloc;
    }
    printf("1\n");
    bool is_read_cmd = false;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            fclose(in);
            return code_error_alloc;
        }
        printf("line: %s\n", line);
        printf("2\n");
        symbol = line[index];
        while(isspace(symbol)) {
            index++;
            symbol = line[index];
        }
        printf("3\n");
        int index_cmd = 0;
        while(isalpha(symbol)) {
            symbol = line[index];
            if (symbol == '(') {
                break;
            }
            cmd[index_cmd] = symbol;
            index++;
            index_cmd++;
        }
        printf("4\n");
        if (index_cmd != 0) {
            cmd[index_cmd] = '\0';
            index_cmd = 0;
        }
        printf("5\n");
        int size_expr = strlen(line);
        char* expr_bracket = (char*)malloc(sizeof(char) * size_expr);
        if (!expr_bracket) {
            free(cmd);
            free(line);
            fclose(in);
            return code_invalid_parameter;
        }
        printf("6\n");
        while(symbol != '%' || !isspace(symbol)) {
            symbol = line[index];
            printf("%c %d\n", symbol, index);
            if (symbol == '%' || isspace(symbol) || symbol == '\r' || symbol == '\n') {
                expr_bracket[index_cmd] = '\0';
                break;
            }
            //printf("%c", symbol);
            expr_bracket[index_cmd] = symbol;
            index++;
            index_cmd++;
        }
        expr_bracket[index_cmd] = '\0';
        index_cmd = 0;
        printf("cmd    %s\n", cmd);
        printf("    %s\n", expr_bracket);
        free(line);
        printf("7\n");
        line = NULL;
        Polynom* first = NULL;
        Polynom* second = NULL;
        double point;
        printf("8\n");
        if (strcmp(cmd, "Eval")) {
            printf("8\n");
            st_act = create_polynomial(&first, &second, expr_bracket);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                break;
            }
        } else {
            if (sscanf(expr_bracket, "(%lf)", &point) != 1) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                fclose(in);
                break;
            }
        }
        printf("9\n");
        double res_eval = 0;
        Polynom* tmp_res = NULL;
        if (!second) {
            st_act = action(cmd, first, summator, &tmp_res, point, &res_eval);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                break;
            }
        } else {
            printf("this\n");
            st_act = action(cmd, first, second, &tmp_res, point, &res_eval);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                break;
            }
            /*
            printf("1\n");
            print_polynom(first);
            printf("2\n");
            print_polynom(second);
            printf("res\n");
            print_polynom(tmp_res);
            */
           print_polynom(tmp_res);
        }
        printf("10\n");
        destroy_polynomial(summator);
        summator = NULL;
        summator = tmp_res;
        
    }
    if (line) free(line);
    fclose(in);
    return code_success;
}

bool is_operator(char symbol) {
    return symbol == '+' || symbol == '-';
}



status_code create_polynomial(Polynom** first, Polynom** second, char* expression) {
    if (!expression) return code_invalid_parameter;
    int size = strlen(expression);
   // printf("%s %d\n", expression, size);
    //printf("8.1\n");
    //printf("%c - %c\n", expression[0], expression[size - 1]);
    if (expression[0] != '(' || expression[size - 1] != ')') {
        return code_invalid_parameter;
    }
    //printf("8.2\n");
    char symbol;
    bool is_read_coef = false;
    bool is_read_letter = false;
    bool is_read_degree = false;
    bool is_second_pol = false;
    bool is_negative = false;
    status_code st_act;
    int coef;
    int degree;
    //printf("8.3\n");
    char coef_str[MAX_SIZE];
    char degr_str[MAX_SIZE];
    int tmp_index = 0;
    for (int i = 1; i < size - 1; i++) {
        symbol = expression[i];
        //printf("    symbol: %c\n", symbol);
        if (symbol == ',') {
            is_second_pol = true;
        }
        if (symbol == 'x') {
            if (!is_read_coef) {
                coef = 1;
                if (is_negative) {
                    coef *= -1;
                    is_negative = false;
                }
                is_read_coef = true;
                //printf("coef = %d\n", coef);
            }
            if (expression[i + 1] != '^') {
                degree = 1;
                is_read_degree = true;
            }
            if (is_read_letter) return code_invalid_parameter;
            is_read_letter = true;
        }
        if (isdigit(symbol)) {
            if (!is_read_coef) {
                //printf("~~~~~~~~here~~~~~~~~\n");
                coef_str[tmp_index] = symbol;
                tmp_index++;
                if (!isdigit(expression[i + 1])) {
                    coef = atoi(coef_str);
                    if (is_negative) {
                        coef *= -1;
                        is_negative = false;
                    }
                    //printf("-----%d\n", coef);
                    degr_str[0] = '\0';
                    tmp_index = 0;
                    is_read_coef = true;
                }
                if (expression[i + 1] != 'x') {
                    //printf("~~~~~~~~here~~~~~~~~\n");
                    degree = 0;
                    is_read_degree = true;
                    is_read_letter = true;
                }
            } else {
                if (!is_read_degree) {
                    degr_str[tmp_index] = symbol;
                    tmp_index++;
                    if (!isdigit(expression[i + 1])) {
                        degree = atoi(degr_str);
                        if (is_negative) {
                            coef *= -1;
                            is_negative = false;
                        }
                        //printf("-----%d\n", degree);
                        degr_str[0] = '\0';
                        tmp_index = 0;
                        is_read_degree = true;
                    }
                }
            }
        }
        if (is_operator(symbol)) {
            if (symbol == '-') {
                //printf("reaL NIG\n");
                is_negative = true;
            }
        }
        if (is_read_degree && is_read_coef && is_read_letter) {
            if (!is_second_pol) {
                st_act = push_monom(first, degree, coef);
                if (st_act != code_success) {
                    return st_act;
                }
            } else {
                st_act = push_monom(second, degree, coef);
                if (st_act != code_success) {
                    return st_act;
                }
            }
            is_read_degree = false;
            is_read_coef = false;
            is_read_letter = false;
        }
        /*
        printf("+++++++++++++++first+++++++++++++++++\n");
        print_polynom(*first);
        printf("+++++++++++++++++++++++++++++++++++++\n");
        printf("+++++++++++++++second++++++++++++++++\n");
        print_polynom(*second);
        printf("+++++++++++++++++++++++++++++++++++++\n");
        */
        
        
    }
    //print_polynom(*first);
    //print_polynom(*second);
    //printf("woow\n");
    return code_success;
}

status_code copy_monom(Polynom* monom, Polynom** cpy_monom) {
    if (!monom) return code_invalid_parameter;
    (*cpy_monom) = (Polynom*)malloc(sizeof(Polynom));
    if (!(*cpy_monom)) return code_error_alloc;
    (*cpy_monom)->coef = monom->coef;
    (*cpy_monom)->degree = monom->degree;
    return code_success;
}

void print_polynom_another(Polynom* equation) {
    if (!equation) return;
    Polynom* current = equation;
    while (current != NULL) {
        printf("coef: %d, degree: %d\n", current->coef, current->degree);
        current = current->next;
    }
    printf("\n");
}

status_code add(Polynom* first, Polynom* second, Polynom** res) {
    if (!first) return code_invalid_parameter;
    Polynom* left_cur = first;
    Polynom* right_cur = second;
    status_code st_act;
    int coef;
    while (left_cur != NULL && right_cur != NULL) {
        if (left_cur->degree == right_cur->degree) {
            coef = left_cur->coef + right_cur->coef;
            if (coef != 0) {
                st_act = push_monom(res, left_cur->degree, coef);
                if (st_act != code_success) return st_act;
            }
            left_cur = left_cur->next;
            right_cur = right_cur->next;
            //printf("    %d + %d coeffff %d\n", left_cur->coef,right_cur->coef,left_cur->coef + right_cur->coef);
        } else if (left_cur->degree < right_cur->degree) {
            st_act = push_monom(res, left_cur->degree, left_cur->coef);
            if (st_act != code_success) return st_act;
            left_cur = left_cur->next;
        } else if (left_cur->degree > right_cur->degree) {
            st_act = push_monom(res, right_cur->degree, right_cur->coef);
            if (st_act != code_success) return st_act;
            right_cur = right_cur->next;
        }
    }
    while (left_cur != NULL) {
        st_act = push_monom(res, left_cur->degree, left_cur->coef);
        //printf("<<<%d %d>>>\n", left_cur->degree, left_cur->coef);
        if (st_act != code_success) return st_act;
        left_cur = left_cur->next;
    }
    while (right_cur != NULL) {
        st_act = push_monom(res, right_cur->degree, right_cur->coef);
        if (st_act != code_success) return st_act;
        right_cur = right_cur->next;
    }
    //printf("\n---\n");
    //print_polynom(*res);
    //printf("\n---\n");
    return code_success;
}

status_code sub(Polynom* first, Polynom* second, Polynom** res) {
    if (!first) return code_invalid_parameter;
    Polynom* left_cur = first;
    Polynom* right_cur = second;
    status_code st_act;
    int coef;
    while (left_cur != NULL && right_cur != NULL) {
        if (left_cur->degree == right_cur->degree) {
            coef = left_cur->coef - right_cur->coef;
            if (coef != 0) {
                st_act = push_monom(res, left_cur->degree, coef);
                if (st_act != code_success) return st_act;
            }
            left_cur = left_cur->next;
            right_cur = right_cur->next;
        } else if (left_cur->degree < right_cur->degree) {
            st_act = push_monom(res, left_cur->degree, left_cur->coef);
            if (st_act != code_success) return st_act;
            left_cur = left_cur->next;
        } else if (left_cur->degree > right_cur->degree) {
            st_act = push_monom(res, right_cur->degree, -right_cur->coef);
            if (st_act != code_success) return st_act;
            right_cur = right_cur->next;
        }
    }
    while (left_cur != NULL) {
        st_act = push_monom(res, left_cur->degree, left_cur->coef);
        //printf("<<<%d %d>>>\n", left_cur->degree, left_cur->coef);
        if (st_act != code_success) return st_act;
        left_cur = left_cur->next;
    }
    while (right_cur != NULL) {
        st_act = push_monom(res, right_cur->degree, -right_cur->coef);
        if (st_act != code_success) return st_act;
        right_cur = right_cur->next;
    }
    //printf("\n---\n");
    print_polynom(*res);
    //printf("\n---\n");
    return code_success;
}

status_code mult(Polynom* first, Polynom* second, Polynom** res) {
    Polynom* left_cur = first;
    Polynom* right_cur = second;
    status_code st_act;
    while (left_cur != NULL && right_cur != NULL) {
        Polynom* new = NULL;
        if (left_cur->degree == right_cur->degree) {
            st_act = push_monom(res, left_cur->degree * right_cur->degree, left_cur->coef * right_cur->coef );
            if (st_act != code_success) return st_act;
            left_cur = left_cur->next;
            right_cur = right_cur->next;
        } else if (left_cur->degree < right_cur->degree) {
            st_act = push_monom(res, left_cur->degree, left_cur->coef);
            if (st_act != code_success) return st_act;
            left_cur = left_cur->next;
        } else if (left_cur->degree > right_cur->degree) {
            st_act = push_monom(res, right_cur->degree, right_cur->coef);
            if (st_act != code_success) return st_act;
            left_cur = right_cur->next;
        }
    }
    return code_success;
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

status_code eval(Polynom* eq, double point, double* res) {
    if (point == 0 || !eq) {
        *res = 0;
        return code_success;
    }
    *res = 0;
    Polynom* current = eq;
    while (current != NULL) {
        *res += binary_pow(point, current->degree) * current->coef;
        current = current->next;
    }
    return code_success;
}

status_code diff(Polynom* eq) {
    if (!eq) return code_success;
    status_code st_rec;
    if (eq->degree != 0) {
        eq->coef *= eq->degree;
        eq->degree--;
    } else {
        eq->coef = 0;
        eq->degree = 0;
    }
    st_rec = diff(eq->next);
    return st_rec;
}

status_code cmps(Polynom* F, Polynom* G, Polynom** res) {
    if (!F || !G) return code_invalid_parameter;
    Polynom* g_current = G;
    Polynom* f_current = F;
    status_code st_act;
    Polynom* summator = NULL;
    while (g_current != NULL) {
        Polynom* tmp_res = NULL;
        while (f_current != NULL) {
            Polynom* tmp = NULL;
            st_act = copy_monom(g_current, &tmp);
            if (st_act != code_success) return st_act;
            if (f_current->degree != 0) {
                tmp->degree *= f_current->degree;
                tmp->coef = pow(tmp->coef, f_current->degree) * f_current->coef;
            } else {
                tmp->coef = f_current->coef;
                tmp->degree = f_current->degree;
            }
            st_act = push_monom(&tmp_res, tmp->degree, tmp->coef);
            if (st_act != code_success) return st_act;
            f_current = f_current->next;
        }
        if (!summator) {
            summator = tmp_res;
        } else {
            Polynom* calc_add = NULL;
            st_act = add(summator, tmp_res, &calc_add);
            destroy_polynomial(tmp_res);
            destroy_polynomial(summator);
            summator = NULL;
            if (st_act != code_success) {
                return st_act;
            }
            summator = calc_add;
        }

        g_current = g_current->next;
    }

}



status_code action(char* cmd, Polynom* first, Polynom* second, Polynom** res, double point, double* res_sol) {
    if (!first || !second || !cmd) return code_invalid_parameter;

    status_code st_act;
    if (!strcmp(cmd, "Add")) {
        st_act = add(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Sub")) {
        printf("sub\n");
        st_act = sub(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Div")) {
        st_act = add(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Mult")) {
        st_act = mult(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Eval")) {
        st_act = eval(first, point, res_sol);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Diff")) {
        st_act = diff(first);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Cmps")) {
        st_act = cmps(first, second, res);
        if (st_act != code_success) return st_act;
    } else {
        return code_invalid_parameter;
    }
}