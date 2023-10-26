#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

int main(int argc, char* argv[]) {
    FILE* in = fopen("tmp.txt", "r");
    char s[STR_SIZE];
    int num1, num2, num3;
    unsigned int num4;
    overfscanf(in, "%CV %Zr %s %Cv %Ro", &num1, 16, &num4, s, &num2, 16, &num3);   //ABCDEF 16 010011 SUKA abcdef MMCXXXVs
    printf("____%d %d %s %d %d\n", num1, num4, s, num2, num3);
    oversscanf("ABCDEF 010011 SUKA abcdef MMCXXXV", "%CV %Zr %s %Cv %Ro", &num1, 16, &num4, s, &num2, 16, &num3);
    printf(">>>>%d %d %s %d %d\n", num1, num4, s, num2, num3);
    fclose(in);
}