#include "../headers/lab.h"

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

long double function_pi_limit(double eps) {
    long double n = 1;
    long double res = 4;
    long double prev = 0;
    do {
        prev = res;
        res *= 4.0 * ((n + 1) * n) / pow(2 * n + 1, 2);
        n++;
    } while (fabs(res - prev) > eps);
    return res;
}

long double function_gamma_limit(double epsilon) {
    int n = 2;
    long double result  = 1.0;
    long double prev = 0.0;
    do {
        prev = result;
        result = 0;
        for (int i = 1; i <= n; i++) {
            result += 1.0 / i;
        }
        result -= log(n);
        n++;
        
    } while (fabs(result - prev) > epsilon);
    return result;
}

double function_gamma_row() {
    double epsilon = 1e-13;
    double pi = acos(-1);
    double res = 0.5;
    double prev = 0.0;
    double tmp_res = 0.0;
    for (int k = 3; fabs(res - prev) > epsilon; k++) {
        prev = res;
        tmp_res = ((1.0 / pow(floor(sqrt(k)), 2)) - (1.0 / k));
        res += tmp_res;
        if (tmp_res < epsilon) {
            prev = 0.0;
        }
    } 
    return res - pi * pi/ 6.0;
}

status_code function_gamma_equation(double epsilon, double* result) {
    double tmp_res = -log(0.5 * log(2));
    double prev;
    double prod = 0.5;
    int t = 3;
    for (t = 3; fabs(tmp_res - prev) > epsilon; t++) {
        prev = tmp_res;
        tmp_res = -log(log(t));
    }
    int size = 0;
    int* prime_nums = (int*)malloc(16 * sizeof(int));
    if (prime_nums == NULL) {
        return code_error_malloc;
    }
    prime_nums[0] = 2;
    size = 1;
    bool is_prime;
    for (int i = 3; i * i <= t; i += 2) {
        is_prime = true;
        for (int j = 0; j < size; j++) {
            if (i % prime_nums[j] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_nums[size] = i;
            prod *= (prime_nums[size - 1] - 1.0) / prime_nums[size - 1];
            size++;
            if (size > sizeof(prime_nums) - 1) {
                prime_nums = realloc(prime_nums, sizeof(int) * size * 2);
                if (prime_nums == NULL) {
                    free(prime_nums);
                    return code_error_malloc;
                }
            }
        } else {
            continue;
        }
    }
    tmp_res -= log(prod);
    *result = tmp_res;
    free(prime_nums);
    return code_success;
}
