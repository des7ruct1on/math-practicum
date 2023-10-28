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
    /*
    overfscanf(in, "%CV %Zr %s %Cv %Ro", &num1, 16, &num4, s, &num2, 16, &num3);   //ABCDEF 16 010011 SUKA abcdef MMCXXXVs
    printf("____%d %d %s %d %d\n", num1, num4, s, num2, num3);
    oversscanf("ABCDEF 010011 SUKA abcdef MMCXXXV", "%CV %Zr %s %Cv %Ro", &num1, 2, &num4, s, &num2, 16, &num3);
    printf(">>>>%d %d %s %d %d\n", num1, num4, s, num2, num3);
    */
    oversscanf("XV X 1000\0", "%Ro %Ro %CV", &num1, &num2, &num3, 2);
    //overfscanf(in, "%Ro %Ro %Cv", &num1, &num2, &num3, 2);
    printf("%d -- %d -- %d\n", num1, num2, num3);
    int num5, num6;
    oversscanf("XV 1000 X 10110 10001\0", "%Ro %CV %Ro %CV %CV", &num1, &num3, 2, &num2, &num5, 2, &num6, 2);
    printf("%d -- %d -- %d %d %d\n", num1, num2, num3, num5, num6);
    fclose(in);
}