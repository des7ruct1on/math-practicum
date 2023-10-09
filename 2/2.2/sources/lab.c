#include "../headers/lab.h"

status_code geometric_mean(double* res, int count,...) {
    if (count < 1) {
        return code_invalid_parameter;
    }
    va_list ptr;
    *res = 1;
    va_start(ptr, count);
    for (int i = 0; i < count; i++) {
        double value = va_arg(ptr, double);
        *res *= value;
    }
    *res = pow(*res, 1.0 / count);
    va_end(ptr);
    if (isinf(*res)) {
        return code_overflow;
    }
    if (isnan(*res)) {
        return code_nan;
    }

    return code_success;
}

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