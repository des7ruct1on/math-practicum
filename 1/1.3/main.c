#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "headers/lab.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <FLAG> <PARAMETRES>\n", argv[0]);
        exit(1);
    }
    if (argv[1][0] != '/' && argv[1][0] != '-') {
        printf("Your flag must start with '-' or '/' symbol!\n");
        exit(1);
    }
    char flag = argv[1][1];
    double epsilon_q;
    bool result;
    bool need_stop;
    switch (flag) {
        case 'q':
            if (argc != 6) {
                printf("Error: wrong quantity for flag <q>\n");
                exit(1);
            }
            epsilon_q = atof(argv[2]);
            double coef1 = atof(argv[3]);
            double coef2 = atof(argv[4]);
            double coef3 = atof(argv[5]);
            double coefs[3] = {coef1, coef2, coef3};
            bubble_sort_descending(coefs, 3, epsilon_q);
            double root1, root2;
            do {
                printf("Equation: %.3f.*x^2 + (%.3f*x) + (%.3f)\n", coefs[0], coefs[1], coefs[2]);
                switch(solve_sqr_equation(coefs, epsilon_q, &root1, &root2)) {
                    case status_invalid_parametr:
                        printf("Invalid parametr detected!!!\n");
                        break;
                    case status_two_roots:
                        printf("\t\tSolution: x_1 = %f, x_2 = %f\n", root1, root2);
                        break;
                    case status_one_root:
                        printf("\t\tSolution: x = %f\n", root1);
                        break;
                    case status_no_roots:
                        printf("\t\tThis equation does not have solutin in Real numbers\n");
                        break;
                }
            } while(next_permutation(coefs, 3));
            break;
        case 'm':
            if (argc != 4) {
                printf("Error: wrong quantity for flag <m>\n");
                exit(1);
            }
            int num1 = atoi(argv[2]);
            int num2 = atoi(argv[3]);
            switch(is_multiply(num1, num2, &result)) {
                case code_succes:
                    if (result) {
                        printf("Yes, %d is a multiple of %d\n", num1, num2);
                    } else {
                        printf("No, %d is not a multiple of %d\n", num1, num2);
                    }
                    break;
                case code_invalid_parameter:
                    printf("Invalid parametr detected!!!\n");
                    break;
            }
            break;
        case 't':
            if (argc != 6) {
                printf("Error: wrong quantity for flag <t>\n");
                exit(1);
            }
            double epsilon_q = atof(argv[2]);
            double side1 = atof(argv[3]);
            double side2 = atof(argv[4]);
            double side3 = atof(argv[5]);
            double sides[3] = {side1, side2, side3};
            bubble_sort_descending(sides, 3, epsilon_q);
            do {
                switch(is_triangle(sides, epsilon_q, &result)) {
                    case code_succes:
                        if (result) {
                            printf("Yes, if c = %f, a = %f, b = %f\n", sides[0], sides[1], sides[2]);
                            need_stop = true;
                        } else {
                            printf("No, if c = %f, a = %f, b = %f\n", sides[0], sides[1], sides[2]);
                        }
                        break;
                    case code_invalid_parameter:
                        printf("Invalid parametr detected!!!\n");
                        need_stop = true;
                        break;
                }
                if (result || need_stop) {
                    break;
                }
            } while(next_permutation(sides, 3));
            break;
        default:
            printf("Unknown flag! Try again\n");
            break;
    }


    return 0;
}