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
    bool start = true;
    while (current != NULL) {
        if (!start) {
            if (current->coef > 0) {
                printf("+ ");
            }
        }
        if (current->coef != 1 && current->coef != -1 && current->degree != 0) printf("%d", current->coef);
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
            printf("%d ", current->coef);
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
        if (!strcmp(line, "\n") || !strcmp(line, "\r\n")) {
            printf("111\n");
            free(line);
            line = NULL;
            continue;
        }
        printf("%s\n", line);
        index = 0;
        symbol = line[index];
        while (isspace(symbol)) {
            symbol = line[index];
            if (isspace(symbol)) index++;
        }
        if (symbol == '%' || symbol == '\0') {
            free(line);
            line = NULL;
            index = 0;
            continue;
        }
        if (symbol == '[' || is_long_comment) {
            is_long_comment = true;
            while (is_long_comment) {
                symbol = line[index];
                index++;
                if (symbol == ']') {
                    is_long_comment = false;
                    index++;
                    symbol = line[index];
                    //printf("finally\n");
                    break;
                }
                if (symbol == '\n') {
                    break;
                }
            }
        }
        if (is_long_comment) {
            free(line);
            line = NULL;
            printf("notttt\n");
            index = 0;
            continue;
        }
        while (isspace(symbol)) {
            symbol = line[index];
            if (isspace(symbol)) index++;
        }
        int index_cmd = 0;
        while(isalpha(symbol)) {
            symbol = line[index];
            if (isalpha(symbol)) {
                cmd[index_cmd] = symbol;
                index++;
                index_cmd++;
            }
        }
        printf("index cmd: %d\n", index_cmd);
        cmd[index_cmd] = '\0';
        printf("    cmd: %s\n", cmd);
        //printf("    cmd: %s\n", cmd);
        //printf("    cmd: %s\n", cmd);
        if (cmd[0] == '\0' && symbol == '%') {
            //printf("yes\n");
            free(line);
            free(cmd);
            line = NULL;
            cmd = NULL;
            index = 0;
            cmd = (char*)malloc(sizeof(char) * MAX_SIZE);
            if (!cmd) {
                return code_error_alloc;
            }
            continue;
        }
        //printf("<<%d??\n", is_long_comment);
        index_cmd = 0;
        char* expr_bracket = (char*)malloc(sizeof(char) * 256);
        if (!expr_bracket) {
            free(line);
            free(cmd);
            fclose(in);
            return code_invalid_parameter;
        }
        if (symbol != '(') {
            if (symbol == '%') {
                free(line);
                free(cmd);
                free(expr_bracket);
                fclose(in);
                return code_invalid_parameter;
            }
            if (symbol == '[') {
                is_long_comment = true;
                while (is_long_comment) {
                    symbol = line[index];
                    index++;
                    if (symbol == ']') {
                        is_long_comment = false;
                        break;
                    }
                    if (symbol == '\n') {
                        break;
                    }
                }
            }
            
        }
        if (symbol == '(') {
            while (true) {
                symbol = line[index];
                index++;
                if (symbol == '\n' || isspace(symbol) || symbol == '%') {
                    expr_bracket[index_cmd] = '\0';
                    break;
                }
                expr_bracket[index_cmd] = symbol;
                index_cmd++;
            }
        } else {
            if (symbol == '\0') {
                free(line);
                free(cmd);
                cmd = NULL;
                line = NULL;
                free(expr_bracket);
                index = 0;
                cmd = (char*)malloc(sizeof(char) * MAX_SIZE);
                if (!cmd) {
                    return code_error_alloc;
                }
                continue;
            }
            free(line);
            free(cmd);
            free(expr_bracket);
            fclose(in);
            return code_invalid_parameter;
        }
        Polynom* first = NULL;
        Polynom* second = NULL;
        double point;
        double res_val;
        //printf("cmd: %s\n", cmd);
        //printf("brack expr: %s\n", expr_bracket);
        if (strcmp(cmd, "Eval")) {
            //printf("8\n");
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
                //printf("kek\n");
                break;
            }
        }
        Polynom* tmp_res = NULL;
        if (!second) {
            st_act = action(cmd, summator, first, &tmp_res, point, &res_val);
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
            st_act = action(cmd, first, second, &tmp_res, point, &res_val);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                break;
            }
            destroy_polynomial(first);
            destroy_polynomial(second);
        }
        if (!strcmp(cmd, "Eval")) printf("result: %lf\n", res_val);
        destroy_polynomial(summator);
        summator = tmp_res;
        print_polynom(summator);
        if (line) {
            free(line);
            line = NULL;
        }
        if (cmd) free(cmd);
        cmd = (char*)malloc(sizeof(char) * MAX_SIZE);
        if (!cmd) {
            return code_error_alloc;
        }
    }
    if (summator) destroy_polynomial(summator);
    if (line) free(line);
    if (cmd) free(cmd);
    fclose(in);
    if (is_long_comment) return code_invalid_parameter;
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
            //printf("cur degree %d\n", degree);
            if (is_read_letter) return code_invalid_parameter;
            is_read_letter = true;
        }
        if (isdigit(symbol)) {
            if (!is_read_coef) {
                //printf("~~~~~~~~here~~~~~~~~\n");
                coef_str[tmp_index] = symbol;
                tmp_index++;
                if (!isdigit(expression[i + 1])) {
                    //printf("%c-----\n", expression[i + 1]);
                    coef = atoi(coef_str);
                    if (is_negative) {
                        coef *= -1;
                        is_negative = false;
                    }
                    //printf("-----%d\n", coef);
                    coef_str[0] = '\0';
                    tmp_index = 0;
                    is_read_coef = true;
                    //printf("%c %d %d symb\n", symbol, is_read_coef, is_read_letter);
                }
                if (expression[i + 1] != 'x' && !isdigit(expression[i + 1])) {
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
        //printf("%d %d %d\n", is_read_degree, is_read_coef, is_read_letter);
        if (is_read_degree && is_read_coef && is_read_letter) {
            //printf("%d %d\n", degree, coef);
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
    //if (!first) return code_invalid_parameter;
    Polynom* left_cur = first;
    Polynom* right_cur = second;
    status_code st_act;
    int coef;
    if (!second) {
        //printf("wowo\n");
        while (left_cur != NULL) {
            coef = left_cur->coef;
           // printf("%d >> %d\n", left_cur->degree, left_cur->coef);
            st_act = push_monom(res, left_cur->degree, coef);
            if (st_act != code_success) return st_act; 
            left_cur = left_cur->next;
        }
    } else {
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
    }
    
    //printf("\n---\n");
    //print_polynom(*res);
    //print_polynom_another(*res);
    //printf("\n---\n");
    return code_success;
}

status_code sub(Polynom* first, Polynom* second, Polynom** res) {
    //if (!first) return code_invalid_parameter;
    Polynom* left_cur = first;
    Polynom* right_cur = second;
    status_code st_act;
    int coef;
    if (!second) {
        while (left_cur != NULL) {
            coef = 0 - left_cur->coef;
            st_act = push_monom(res, left_cur->degree, coef);
            if (st_act != code_success) return st_act; 
            left_cur = left_cur->next;
        }
    } else {
        while (left_cur != NULL && right_cur != NULL) {
            if (left_cur->degree == right_cur->degree) {
                coef = left_cur->coef - right_cur->coef;
                //printf("%d>>>>\n", coef);
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
    }
    //printf("\n---\n");
    //printf("\n---\n");
    return code_success;
}

status_code mult(Polynom* first, Polynom* second, Polynom** res) {
    if (!first && !second) return code_invalid_parameter;
    Polynom* left_cur = first;

    //printf("-----------\n");

    status_code st_act;
    int degree, coef;
    Polynom* summator = NULL;
    Polynom* sum_res = NULL;
    //printf("start\n");
    while (left_cur != NULL) {
        Polynom* tmp_res = NULL;
        Polynom* right_cur = second;
        while (right_cur != NULL) {
            coef = left_cur->coef * right_cur->coef;
            degree = left_cur->degree + right_cur->degree;
            //printf("coef = %d, degree = %d\n", coef, degree);
            st_act = push_monom(&tmp_res, degree, coef);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                return st_act;
            }
            right_cur = right_cur->next;
        }
        //printf("111111\n");
        //print_polynom(tmp_res);
        st_act = add(tmp_res, summator, &sum_res);
        if (st_act != code_success) {
            destroy_polynomial(tmp_res);
            destroy_polynomial(summator);
            destroy_polynomial(sum_res);
            return st_act;
        }
        //print_polynom(sum_res);
        destroy_polynomial(tmp_res);
        destroy_polynomial(summator);
        summator = sum_res;
        //free(sum_res);
        sum_res = NULL;
        left_cur = left_cur->next;
    }
    //print_polynom(summator);
    *res = summator;
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
    if (!eq) {
        *res = 0;
        return code_success;
    }
    *res = 0;
    Polynom* current = eq;
    while (current != NULL) {

        *res += binary_pow(point, current->degree) * current->coef;
        //printf("(%lf)\n", binary_pow(point, current->degree) * current->coef);
        current = current->next;
    }
    return code_success;
}

status_code diff(Polynom* eq, Polynom** res) {
    if (!eq) return code_success;
    Polynom* current = eq;
    int coef, degree;
    status_code st_act;
    while (current != NULL) {
        degree = current->degree - 1;
        coef = current->coef * current->degree;
        //printf("            %d\n", coef);
        if (degree != -1) {
            st_act = push_monom(res, degree, coef);
            if (st_act != code_success) return st_act;
        }

        current = current->next;
    }
    
    return code_success;
}


status_code cmps(Polynom* F, Polynom* G, Polynom** res) {
    if (!F) return code_success;
    if (!G) return code_invalid_parameter;
    Polynom* summator = NULL;
    Polynom* tmp_res = NULL;
    Polynom* cur_left = F;
    int coef, degree;
    status_code st_act;
    while (cur_left != NULL) {
        int f_degree = cur_left->degree;
        Polynom* cur_g = G;
        Polynom* second_G = NULL;
        st_act = add(G, NULL, &second_G);
        if (st_act != code_success) {
            destroy_polynomial(tmp_res);
            destroy_polynomial(summator);
            destroy_polynomial(second_G);
            return st_act;
        }
        for (int i = 0; i < f_degree - 1; i++) {
            //printf("%d---\n", f_degree);
            st_act = mult(second_G, G, &tmp_res);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
            destroy_polynomial(second_G);
            second_G = tmp_res;
        }
        //printf("kaif\n");
        //print_polynom(tmp_res);
        //printf("kaif\n");
        Polynom* mult_res = NULL;
        if (f_degree == 0) {
            //printf("000\n");
            st_act = push_monom(&mult_res, cur_left->degree, cur_left->coef);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
        } else {
            if (f_degree == 1) {
                tmp_res = second_G;
            }
            st_act = add(tmp_res, summator, &mult_res);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
            //printf("111\n");
            //print_polynom(mult_res);
            //printf("111\n");
        }
        // printf("111\n");
        destroy_polynomial(second_G);
        // printf("111\n");
        destroy_polynomial(summator);
        // printf("111\n");
        //destroy_polynomial(tmp_res);
        // printf("111\n");
        tmp_res = NULL;
        // printf("1112\n");
        summator = mult_res;
        // printf("1113\n");
        tmp_res = NULL;
        // printf("1114\n");
        //printf("cir\n");
        //print_polynom(summator);
        //printf("cir\n");
        cur_left = cur_left->next;
    }
    *res = summator;
    return code_success;

}



status_code action(char* cmd, Polynom* first, Polynom* second, Polynom** res, double point, double* res_sol) {
    if (!cmd) return code_invalid_parameter;
    status_code st_act;
    if (!strcmp(cmd, "Add")) {
        st_act = add(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Sub")) {
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
        st_act = diff(first, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Cmps")) {
        st_act = cmps(first, second, res);
        if (st_act != code_success) return st_act;
    } else {
        return code_invalid_parameter;
    }
    return code_success;
}