#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/tree.h"



int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <FILE>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    status_code st_act;
    st_act = truth_table(filename);
    if (st_act != code_success) {
        print_error(st_act);
    }
    return 0;
}