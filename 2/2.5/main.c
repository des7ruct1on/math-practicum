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
    FILE* in = fopen("tmp.txt", "w");
    overfprintf(stdout, "wow number is: %Ro, %Cv\n", 5435, 1234, 16);
    overfprintf(stdout, "sfdsdf %to %TO %d%d\n", "abcdef", 16, "10", 10, 12, 12);
    overfprintf(stdout, "%mi %mu %md, %mf\n", (int)10, (unsigned int)12, (double)2.2, (double)2.3);
    overfprintf(stdout, "fsdwow number is: %Zr\n", (unsigned int)10);
    overfprintf(in, "fsdwow number is: %Zk\n", (unsigned int)10);
    overfprintf(stdout, "%Ro %CV %Ro %CV %CV\n", "XV", "1000", 2, "X", "10110", 2, "10001", 2);
    char* buf = (char*)malloc(sizeof(char) * 32);
    printf("\n\n");
    oversprintf(buf, "wow number is: %Ro, %Cv\n", 5435, 1234, 16);
    printf("%s\n", buf);
    oversprintf(buf, "sfdsdf %to %TO %d%d\n", "abcdef", 16, "10", 10, 12, 12);
    printf("%s\n", buf);
    oversprintf(buf, "%mi %mu fsd ad fsd %md, %mf\n", (int)10, (unsigned int)12, (double)2.2, (double)2.3);
    printf("%s\n", buf);
    oversprintf(buf, "fsdwow number is: %Zr\n", (unsigned int)10);
    printf("%s\n", buf);
    free(buf);
    fclose(in);
}