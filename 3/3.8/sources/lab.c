#include "../headers/lab.h"

int compare_monoms(Polynom* a, Polynom* b) {
    return  a->degree - b->degree;
}

status_code push_monom(Polynom** equation, int _degree, int _coef) {
    if (_degree == 0 && _coef == 0) return code_success;
    Polynom* new = (Polynom*)malloc(sizeof(Polynom));
    if (!new) return code_error_alloc;
    new->coef = _coef;
    new->degree = _degree;
    if (!(*equation)) {
        *equation = new;
        new->next = NULL;
        
    } else {
        if (!(*equation)->next) {
            if (compare_monoms((*equation), new) > 0) {
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
            if (check <= 0) {
                new->next = current;
                *equation = new;
                return code_success;
            }
            while (current->next != NULL) {
                check = compare_monoms(current->next, new);
                if (check < 0) {
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


status_code push_front_monom(Polynom** equation, int degree, int coef) {
    if (degree == 0 && coef == 0) return code_invalid_parameter;
    Polynom* new = (Polynom*)malloc(sizeof(Polynom));
    if (!new) return code_error_alloc;
    new->coef = coef;
    new->degree = degree;
    if (!(*equation)) {
        *equation = new;
        new->next = NULL;
    } else {
        new->next = *equation;
        *equation = new;
    }
    return code_success;
}


status_code push_back_monom(Polynom** equation, int degree, int coef) {
    if (degree == 0 && coef == 0) return code_invalid_parameter;
    Polynom* new = (Polynom*)malloc(sizeof(Polynom));
    if (!new) return code_error_alloc;
    new->coef = coef;
    new->degree = degree;
    if (!(*equation)) {
        *equation = new;
        new->next = NULL;
    } else {
        Polynom* cur = *equation;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new;
        new->next = NULL;
    }
    return code_success;
}


status_code div_mod(Polynom* first, Polynom* second, Polynom** division_res, Polynom** mod_res) {
    //print_polynom(first);
    //print_polynom(second);
    if (!second) return code_invalid_parameter;
    Polynom* first_copy = NULL;
    status_code st_act = add(first, NULL, &first_copy);
    if (st_act != code_success) {
        destroy_polynomial(first_copy);
        return st_act;
    }
    while (first_copy->degree >= second->degree) {
        //printf("%d -> %d\n", first_copy->degree, second->degree);
        int _coef = first_copy->coef / second->coef;
        //printf("%d-- %d\n", _coef, first_copy->degree - second->degree);
        if (_coef == 0) {
            (*mod_res) = first_copy;
            //printf("zxc\n");
            return code_success;
        }

        Polynom* res_div = NULL;
        st_act = push_front_monom(&res_div, first_copy->degree - second->degree, _coef);
        if (st_act != code_success) {
            destroy_polynomial(first_copy);
            //printf("zxc2\n");
            return st_act;
        }
        st_act = push_monom(division_res, first_copy->degree - second->degree, _coef);
        if (st_act != code_success) {
            destroy_polynomial(first_copy);
            //printf("zxc3\n");
            return st_act;
        }
        //printf("current div res:\n");
        //print_polynom(*division_res);

        Polynom* res_mult_divide = NULL;
        st_act = mult(res_div, second, &res_mult_divide);
        if (st_act != code_success) {
            destroy_polynomial(first_copy);
            destroy_polynomial(res_div);
            //printf("zxc4\n");
            return st_act;
        }
        //printf("2222\n");
        Polynom* new_polynom = NULL;
        st_act = sub(first_copy, res_mult_divide, &new_polynom);
        if (st_act != code_success) {
            destroy_polynomial(first_copy);
            destroy_polynomial(res_div);
            destroy_polynomial(res_mult_divide);
            //printf("zxc5\n");
            return st_act;
        }
        destroy_polynomial(first_copy);
        destroy_polynomial(res_mult_divide);
        destroy_polynomial(res_div);
        first_copy = new_polynom;
        //printf("33333\n");
        //printf("33333\n");
        if (!first_copy) {
            break;
        }
    }
    //printf("33343223433\n");
    //printf("mode:\n");
    //print_polynom(first_copy);
    //printf("div:\n");
    //print_polynom(*division_res);
    (*mod_res) = first_copy;
    return code_success;
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
   // printf("1\n");
    bool is_read_cmd = false;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            line = NULL;
            fclose(in);
            return code_error_alloc;
        }
        //printf("%s\n", line);
        if (!strcmp(line, "\n") || !strcmp(line, "\r\n")) {
            //printf("111\n");
            free(line);
            line = NULL;
            continue;
        }
        //printf("%s\n", line);
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
        //printf("%s\n", line);
        if (symbol == '[' || is_long_comment) {
            //printf("\t1\n");
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
        //printf("%s\n", line);
        if (is_long_comment) {
            free(line);
            line = NULL;
            //printf("notttt\n");
            index = 0;
            continue;
        }
        //printf("%s\n", line);
        while (isspace(symbol)) {
            symbol = line[index];
            if (isspace(symbol)) index++;
        }
        //printf("%s\n", line);
        int index_cmd = 0;
        while(isalpha(symbol)) {
            symbol = line[index];
            if (isalpha(symbol)) {
                cmd[index_cmd] = symbol;
                index++;
                index_cmd++;
            }
        }
        //printf("index cmd: %d\n", index_cmd);
        cmd[index_cmd] = '\0';
        //printf("    cmd: %s\n", cmd);
        //printf("    cmd: %s\n", cmd);
        //printf("    cmd: %s\n", cmd);
        //printf("%s\n", line);
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
        //printf("%s\n", line);
        //printf("<<%d??\n", is_long_comment);
        index_cmd = 0;
        char* expr_bracket = (char*)malloc(sizeof(char) * 256);
        //printf("9988888\n");
        if (!expr_bracket) {
            free(line);
            free(cmd);
            cmd = NULL;
            line = NULL;
            fclose(in);
            return code_invalid_parameter;
        }
        //printf("9\n");
        if (symbol != '(') {
            //printf("\t1\n");
            if (symbol == '%') {
                free(line);
                free(cmd);
                free(expr_bracket);
                cmd = NULL;
                line = NULL;
                expr_bracket = NULL;
                fclose(in);
                return code_invalid_parameter;
            }
            if (symbol == '[') {
                //printf("\t1\n");
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
                //printf("symbol suka %c %d\n", symbol, toascii(symbol));
                index++;
                if (symbol == '\n' || isspace(symbol) || symbol == '%' || symbol == '\0') {
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
                expr_bracket = NULL;
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
            cmd = NULL;
            line = NULL;
            expr_bracket = NULL;
            fclose(in);
            return code_invalid_parameter;
        }
        Polynom* first = NULL;
        Polynom* second = NULL;
        double point;
        double res_val;
        //printf("8fsdfsdfsd\n");
        if (strcmp(cmd, "Eval")) {
            //printf("8\n");
            //printf("kek\n");
            //printf("kek\n");
            st_act = create_polynomial(&first, &second, expr_bracket);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                expr_bracket = NULL;
                //fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                destroy_polynomial(summator);
                summator = NULL;
                break;
            }
        } else {
            //printf("%s woowowo\n", expr_bracket);
            if (sscanf(expr_bracket, "(%lf)", &point) != 1) {
                free(cmd);
                cmd = NULL;
                free(expr_bracket);
                expr_bracket = NULL;
                //fclose(in);
                destroy_polynomial(first);
                destroy_polynomial(second);
                destroy_polynomial(summator);
                break;
            }
        }
        Polynom* tmp_res = NULL;
        if (!second) {
            //printf("kek\n");
            st_act = action(cmd, summator, first, &tmp_res, point, &res_val);
            if (st_act != code_success) {
                free(cmd);
                cmd = NULL;  
                free(expr_bracket);
                expr_bracket = NULL;
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
                expr_bracket = NULL;
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
        if (cmd) {
            free(cmd);
            cmd = (char*)malloc(sizeof(char) * MAX_SIZE);
            if (!cmd) {
                return code_error_alloc;
            }
        }
        free(expr_bracket);
        expr_bracket = NULL;
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
    //printf("%s %d\n", expression, size);
    //printf("8.1\n");
    //printf("%c - %c\n", expression[0], expression[size - 1]);
    if (expression[0] != '(' || expression[size - 2] != ')') {
        return code_invalid_parameter;
    }
    //printf("8.2\n");
    char* check_end = strchr(expression, ';');
    if (!check_end) return code_invalid_parameter;
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
    for (int i = 1; i < size - 2; i++) {
        symbol = expression[i];
        if (isspace(symbol)) {
            continue;
        }
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
                //printf("    num: %c %d\n", coef_str[tmp_index], isdigit(expression[i + 1]));
                tmp_index++;
                if (!isdigit(expression[i + 1])) {
                    //printf("%c-----\n", expression[i + 1]);
                    coef_str[tmp_index] = '\0';
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
                        degr_str[tmp_index] = '\0';
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
    }
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
    //print_polynom(first);
    //print_polynom(second);
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
                //printf("%d\n", coef);
                if (coef != 0) {
                    st_act = push_monom(res, left_cur->degree, coef);
                    if (st_act != code_success) return st_act;
                }
                left_cur = left_cur->next;
                right_cur = right_cur->next;
            } else if (left_cur->degree < right_cur->degree) {
                st_act = push_monom(res, right_cur->degree, right_cur->coef);
                if (st_act != code_success) return st_act;
                right_cur = right_cur->next;
            } else if (left_cur->degree > right_cur->degree) {
                st_act = push_monom(res, left_cur->degree, left_cur->coef);
                if (st_act != code_success) return st_act;
                left_cur = left_cur->next;
            }
        }
        while (left_cur != NULL) {
            st_act = push_monom(res, left_cur->degree, left_cur->coef);
            if (st_act != code_success) return st_act;
            left_cur = left_cur->next;
        }
        while (right_cur != NULL) {
            st_act = push_monom(res, right_cur->degree, right_cur->coef);
            if (st_act != code_success) return st_act;
            right_cur = right_cur->next;
        }
    }
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
                st_act = push_monom(res, right_cur->degree, -right_cur->coef);
                if (st_act != code_success) return st_act;
                right_cur = right_cur->next;
            } else if (left_cur->degree > right_cur->degree) {
                st_act = push_monom(res, left_cur->degree, left_cur->coef);
                if (st_act != code_success) return st_act;
                left_cur = left_cur->next;
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
    return code_success;
}

status_code mult(Polynom* first, Polynom* second, Polynom** res) {
    if (!first && !second) return code_invalid_parameter;
    Polynom* left_cur = first;

    //printf("-----------\n");
    //print_polynom(first);
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
        //printf("cuurrr\n");
        //print_polynom(summator);
        //printf("~~cuurrr\n");
        int f_degree = cur_left->degree;
        Polynom* cur_g = G;
        Polynom* second_G = NULL;
        st_act = add(G, NULL, &second_G);
        //print_polynom(second_G);
        if (st_act != code_success) {
            destroy_polynomial(tmp_res);
            destroy_polynomial(summator);
            destroy_polynomial(second_G);
            return st_act;
        }
        //printf("%d degree\n", f_degree);
        for (int i = 0; i < f_degree - 1; i++) {
            //printf("    %d---\n", second_G->degree);
            st_act = mult(second_G, G, &tmp_res);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
            destroy_polynomial(second_G);
            second_G = tmp_res;
            //print_polynom(second_G);
            //printf("kaif\n");
        }
        //printf("kaif\n");
        //print_polynom(tmp_res);
        //printf("kaif\n");
        Polynom* mult_res = NULL;
        if (f_degree == 0) {
            //printf("000\n");
            st_act = push_monom(&tmp_res, cur_left->degree, cur_left->coef);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
            st_act = add(tmp_res, summator, &mult_res);
            if (st_act != code_success) {
                destroy_polynomial(tmp_res);
                destroy_polynomial(summator);
                destroy_polynomial(second_G);
                return st_act;
            }
            destroy_polynomial(tmp_res);
            //printf("122211\n");
            //print_polynom(mult_res);
            //printf("122211\n");
        } else {
            if (f_degree == 1) {
                //printf("yes\n");
                print_polynom(second_G);
                tmp_res = second_G;
                tmp_res->coef *= cur_left->coef;
                //printf("yes\n");
                //print_polynom(tmp_res);
                //printf("..yes\n");
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
    //printf("---------------\n");
    //print_polynom(summator);
    //printf("---------------\n");
    return code_success;

}


status_code action(char* cmd, Polynom* first, Polynom* second, Polynom** res, double point, double* res_sol) {
    if (!cmd) return code_invalid_parameter;
    status_code st_act;
    Polynom* tmp = NULL;
    if (!strcmp(cmd, "Add")) {
        st_act = add(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Sub")) {
        st_act = sub(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Div")) {
        st_act = div_mod(first, second, res, &tmp);
        if (st_act != code_success) {
            destroy_polynomial(tmp);
            return st_act;
        }
        //print_polynom(*res);
        destroy_polynomial(tmp);
        tmp = NULL;
    } else if (!strcmp(cmd, "Mod")) {
        st_act = div_mod(first, second, &tmp, res);
        if (st_act != code_success) {
            destroy_polynomial(tmp);
            return st_act;
        }
        destroy_polynomial(tmp);
        tmp = NULL;
        //print_polynom(*res);
    } else if (!strcmp(cmd, "Mult")) {
        st_act = mult(first, second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Eval")) {
        st_act = eval(first, point, res_sol);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Diff")) {
        st_act = diff(second, res);
        if (st_act != code_success) return st_act;
    } else if (!strcmp(cmd, "Cmps")) {
        st_act = cmps(first, second, res);
        if (st_act != code_success) return st_act;
    } else {
        return code_invalid_parameter;
    }
    return code_success;
}