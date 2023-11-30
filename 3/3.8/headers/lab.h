#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAX_SIZE 24

typedef enum status_code {
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef struct Polynom {
    int degree;
    int coef;
    struct Polynom* next;
} Polynom;

int compare_monoms(Polynom* a, Polynom* b);
status_code push_monom(Polynom** equation, int _degree, int _coef);
int size_list(Polynom* list);
void print_polynom(Polynom* equation);
void destroy_polynomial(Polynom* list);
status_code read_from_file(const char* filename);
bool is_operator(char symbol);
status_code create_polynomial(Polynom** first, Polynom** second, char* expression);
status_code copy_monom(Polynom* monom, Polynom** cpy_monom);
status_code add(Polynom* first, Polynom* second, Polynom** res);
status_code sub(Polynom* first, Polynom* second, Polynom** res);
status_code mult(Polynom* first, Polynom* second, Polynom** res);
double binary_pow(double base, int power);
status_code eval(Polynom* eq, double point, double* res);
status_code diff(Polynom* eq);
status_code cmps(Polynom* F, Polynom* G, Polynom** res);
//status_code div(Polynom* F, Polynom* G, Polynom** res);
status_code action(char* cmd, Polynom* first, Polynom* second, Polynom** res, double point, double* res_sol);
#endif