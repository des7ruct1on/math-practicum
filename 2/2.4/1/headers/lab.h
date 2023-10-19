#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

typedef struct Point {
    double x;
    double y;
} Point;

typedef enum status_code {
    code_yes,
    code_no,
    code_invalid_parameter
} status_code;

status_code is_convex_polygon(int count, ...);

#endif