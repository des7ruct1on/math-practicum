#include "../headers/lab.h"

bool compare_double_less(double a, double b, double epsilon) {
    return (a - b) < epsilon;
}

bool compare_double_bigger(double a, double b, double epsilon) {
    return (a - b) > epsilon;
}

bool compare_double_equal(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void swap(double *arr, int i, int j) {
    double temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void reverse(double *arr, int begin, int end) {
    while (begin < end) {
        swap(arr, begin, end);
        begin++;
        end--;
    }
}

void quick_sort(double* arr, int low, int high, double epsilon) {
    if (low < high) {
        double pivot = arr[high]; 
        int i = low - 1; 

        for (int j = low; j <= high - 1; j++) {
            if (arr[j] - epsilon <= pivot) {
                i++; 
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, high);
        quick_sort(arr, low, i, epsilon);
        quick_sort(arr, i + 2, high, epsilon);
    }
}


bool next_permutation(double *arr, int n) {
    int i = n - 2;

    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }

    if (i < 0) {
        return false;
    }

    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }

    swap(arr, i, j);
    reverse(arr, i + 1, n - 1);

    return true;
}

sqr_equation_status_code solve_sqr_equation(double* coefs, double epsilon, double* root1, double* root2) {
    if (compare_double_equal(coefs[0], 0, epsilon)) {
        return status_invalid_parametr;
    }
    double discriminant = coefs[1] * coefs[1] - 4 * coefs[0] * coefs[2];
    if (discriminant > 0) {
        *root1 = ((-1) * coefs[1] + sqrt(discriminant)) / (2 * coefs[0]);
        *root2 = ((-1) * coefs[1] - sqrt(discriminant)) / (2 * coefs[0]);
        if (*root1 == -0) {
            *root1 = 0;
        }
        if (*root2 == -0) {
            *root2 = 0;
        }
        return status_two_roots;
    } else if (discriminant == 0) {
        *root1 = ((-1) * coefs[1]) / (2 * coefs[0]);
        if (*root1 == -0) {
            *root1 = 0;
        }
        return status_one_root;
    } else {
        return status_no_roots;
    }
}

status_code is_multiply(int num1, int num2, bool* result) {
    if ((num1 > 0 && num2 <= 0) || (num1 < 0 && num2 >= 0) || num2 == 0) {
        return code_invalid_parameter;
    }
    if (num1 % num2 == 0) {
        *result = true;
    } else {
        *result = false;
    }
    return code_succes;

}

status_code is_triangle(double* sides, double epsilon, bool* result) {
    if (compare_double_equal(sides[0], 0, epsilon) || compare_double_equal(sides[1], 0, epsilon) || compare_double_equal(sides[2], 0, epsilon)) {
        return code_invalid_parameter;
    }
    if (compare_double_equal(sides[0] * sides[0], sides[1] * sides[1] + sides[2] * sides[2], epsilon)) {
        *result = true;
    } else {
        *result = false;
    }
    return code_succes;
}