#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#define STR_SIZE 256

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

typedef enum status_code {
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

typedef struct Student {
    unsigned int id;
    char name[256];
    char surname[256];
    char group[256];
    unsigned char* marks;
} Student;
status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
status_code read_from_file(FILE* in, Student** storage, int* capacity);
int compare_students(const void* a, const void* b);
void write_file(FILE* out, Student* storage, int size);
bool is_sorted(Student* storage, int size);
double get_avg_ball(Student* student);
void print_table(Student* storage, int size);
void rewrite_file(FILE* out, Student* storage, int size);
Student* get_student_name(Student* storage, int size, int* index, const char* find_name);
Student* get_student_surname(Student* storage, int size, int* index, const char* find_surname);
Student* get_student_id(Student* storage, int size, int* index, unsigned int find_id);
Student* get_student_group(Student* storage, int size, int* index, const char* find_group);
void print_student_chars(FILE* out, Student* stud);
void print_stud_bigger_avg(FILE* out, Student* storage, int size);
#endif