#include "../headers/lab.h"

status_code is_convex_polygon(int count, ...) {
    if (count < 3) {
        return code_invalid_parameter;
    }
    va_list ptr;
    va_start(ptr, count);
    Point prev, next, current;
    prev = va_arg(ptr, Point);
    current = va_arg(ptr, Point);
    next = va_arg(ptr, Point);

    int direction = 0;
    for (int i = 3; i <= count; i++) {
        prev = current;
        current = next;
        next = va_arg(ptr, Point);
        Point vector1, vector2;
        vector1.x = current.x - prev.x;
        vector1.y = current.y - prev.y;
        vector2.x = next.x - current.x;
        vector2.y = next.y - current.y;
        double cross_prod = vector1.x * vector2.y - vector1.y * vector2.x;
        if (cross_prod != 0) {
            if (direction == 0) {
                direction = cross_prod > 0 ? 1 : -1;
            } else if ((cross_prod&& direction == -1) || (cross_prod < 0 && direction == 1)) {
                //printf("vec1: <%f,%f>, vec2: <%f,%f>, %f\n", vector1.x , vector1.y, vector2.x, vector2.y, cross_prod);
                va_end(ptr);
                return code_no; 
            }
        }
    }
    va_end(ptr);
    return code_yes;
}