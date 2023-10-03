#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
//#include "headers/lab.h"

long double limit(long double function(int x), double epsilon) {
    int i = 1;
    int step = 1 / epsilon;
    long double res = 0;
    long double prev_res = 0;
    while (fabs(res - prev_res) > epsilon || prev_res == 0) {
        prev_res = res;
        i += step;
        res = function(i);
    }

    return res;
}


long double summ_row(long double function(int x), double epsilon, int begin) {
    long double res = 0;
    long double prev_res = 0;
    while (fabs(res - prev_res) > epsilon || fabs(prev_res) < epsilon) {
        prev_res = res;
		res += function(begin);
        begin++;
    } 
    return res;
}

long double prod(long double function(int x), double epsilon, int begin) {
    long double res = 1;
    long double prev_res = 0;
    while (fabs(res - prev_res) > epsilon || fabs(prev_res) < epsilon) {
        prev_res = res;
		res *= function(begin);
        begin++;
    }
    return res;
}

long double factorial(int n) {
	if (n == 0) {
		return 1;
	}
	int res = 1;
	while (n > 0) {
		res *= n;
		n--;
	}
	return res;
}

long double function_exp_limit(int x) {
    return pow(1 + 1.0 / x, x);
}

long double function_exp_row(int x) {
    return 1 / factorial(x);
}

long double function_pi_row(int x) {
	return 4 * pow(-1.0, x - 1) / (2.0 * x - 1);
}

long double function_log_2_lim(int x) {
    return x * (pow(2, 1 / x) - 1);
}

long double function_log_2_row(int x) {
    return pow(-1, x - 1) / x;
}

long double function_sqrt2_lim(double epsilon) {
	int i = 0, j;
	long double res = -0.5, prev_res = 0;

	while (fabs(res - prev_res) > epsilon || prev_res == 0) {
		prev_res = res;
		res = res - ((res * res) / 2) + 1;
	}
	return res;
}

double equation_dichotomy(double equation(double x), double a, double b, double eps) {
	double mid_res;
	while (fabs(a - b) > eps) {
		mid_res = equation((a + b) / 2);
		if (equation(a) * mid_res > 0) {
			a = (a + b) / 2;
		}

		if (equation(b) * mid_res > 0) {
			b = (a + b) / 2;
		}
	}
	return (a + b) / 2;
}

long double function_sqrt2_row(int x) {
    return pow(2, pow(2, -x));
}

double function_exp_equation(double x) {
    return log(x) - 1;
}

double function_pi_equation(double x) {
    return sin(x);
}

double function_sqrt_equation(double x) {
    return pow(x, 2) - 2;
}

long double function_pi_limit(int x) {
	int tmp = x;
	long double res = 1;
	while (tmp) {
		res *= 4.0 * tmp * tmp / (2 * tmp) / (2 * tmp - 1);
		tmp--;
	}
	res /= sqrt(x);
	return res *= res;
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Your programm must start with: %s <ACCURACY_PARAM>", argv[0]);
        exit(1);
    }
    int l = atof(argv[1]);
    double epsilon = pow(10, -l);
    printf("e-const\n");
    printf("limit: %.*Lf\n", l, limit(function_exp_limit, epsilon));
    printf("row: %.*Lf\n", l, summ_row(function_exp_row, epsilon, 0));
    printf("equation: %.*f\n", l, equation_dichotomy(function_exp_equation, 2, 3, epsilon));
    printf("pi-const\n");
    printf("limit: %.*Lf\n", l, limit(function_pi_limit, epsilon));
    printf("row: %.*Lf\n", l, summ_row(function_pi_row, epsilon, 1));
    printf("equation: %.*f\n", l, equation_dichotomy(function_pi_equation, 2, 3.5, epsilon));
    printf("sqrt\n");
    printf("limit: %.*Lf\n", l, function_sqrt2_lim(epsilon));
    printf("row: %.*Lf\n", l, prod(function_sqrt2_row, epsilon, 2));
    printf("equation: %.*f\n", l, equation_dichotomy(function_sqrt_equation, 1, 2, epsilon));


    return 0;
}