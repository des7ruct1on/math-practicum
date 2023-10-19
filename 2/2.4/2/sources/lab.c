#include "../headers/lab.h"

status_code binary_pow(double* res, double value, int degree) {
    if (degree == 0) {
        *res = 1.0;
        return code_success;
    }
    if (fabs(value) < EPS && fabs(degree) < EPS) {
        return code_nan;
    }
    double tmp;
    if (degree > 0) {
        status_code tmp_status = binary_pow(&tmp, value, degree / 2);

        if ((degree & 1) == 0) {
            *res = tmp * tmp;
        } else {
            *res = value * tmp * tmp;
        }
    } else {
        status_code tmp_status = binary_pow(&tmp, value, -degree);
        if (tmp_status != code_success) {
            return tmp_status;
        }
        *res = 1.0 / tmp;
    }   
    if (isinf(*res)) {
        return code_overflow;
    }
    if (isnan(*res)) {
        return code_nan;
    }
    return code_success;
}

status_code evaluate_polynomial(double x, double* res, int count,...) {
    if (count < 0) {
        return code_invalid_parameter;
    }
    *res = 0;
    va_list ptr;
    va_start(ptr, count);
    int i = count - 1;
    double tmp_res;
    double high_coeff = va_arg(ptr, double);
    while (!high_coeff) {
        i--;
        high_coeff = va_arg(ptr, double);
    }
    status_code st_res_high_degree = binary_pow(&tmp_res, x, i);
    *res += high_coeff * tmp_res;
    for (i; i >= 0; i--) {
        tmp_res /= x;
        double coeff = va_arg(ptr, double);
        if (fabs(coeff) > EPS) {
            *res += coeff * tmp_res;
        }
    }
    va_end(ptr);
    return code_success;
}