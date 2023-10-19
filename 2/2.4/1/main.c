#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "headers/lab.h"

int main (int argc, const char *argv[]) {
    Point f1_1 = {1, 1};
    Point f1_2 = {5, 2};
    Point f1_3 = {5, 6};
    Point f1_4 = {1, 5};

    Point f2_1 = {0, 6};
    Point f2_2 = {3, 3};
    Point f2_3 = {7, 2};
    Point f2_4 = {0, -5};
    Point f2_5 = {-4, 0};
    Point f2_6 = {-3, 3};

    switch(is_convex_polygon(4, f1_1, f1_2, f1_3, f1_4)) {
        case code_yes:
            printf("Yes, this polygon is convex\n");
            break;
        case code_no:
            printf("No, this polygon is not convex\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
    }

    switch(is_convex_polygon(6, f2_1, f2_2, f2_3, f2_4, f2_5, f2_6)) {
        case code_yes:
            printf("Yes, this polygon is convex\n");
            break;
        case code_no:
            printf("No, this polygon is not convex\n");
            break;
        case code_invalid_parameter:
            printf("Invalid parameter detected!!!\n");
            break;
    }
    return 0;

}