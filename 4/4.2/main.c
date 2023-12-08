#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/tree.h"

void print_menu() {
    printf("\n");
    printf("[Load] <Name, filename> - to load number to Array from file\n");
    printf("[Save] <Name, filename> - to save numbers from Array to file\n");
    printf("[Rand] <Name, count, lb, rb> - to fill array by random numbers in [lb, rb]\n");
    printf("[Concat] <Name_1, Name_2> - concat two arrays\n");
    printf("[Free] <Name> - to free array\n");
    printf("[Remove] <Name, len, index> - to delete <len> numbers after Name[index]\n");
    printf("[Copy] <Name_1, ind_st, ind_end, Name_2> - to copy elements from Name_1 to Name_2\n");
    printf("[Sort] <Name, A+/A-> - to sort array ascending/descending\n");
    printf("[Shuffle] <Name> - to shuffle array\n");
    printf("[Stats] <Name> - to print info about Name\n");
    printf("[Print] <Name, index/diap/all> - to print elements of Name\n");
    printf("[Exit] - to exit the programm\n");
    printf("\n");
}

int main(int argc, char* argv[]) {

    
    return 0;
}