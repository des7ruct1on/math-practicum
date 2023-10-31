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
    code_error_oppening,
    code_success,
    code_invalid_parameter,
    code_error_alloc
} status_code;

typedef enum status_free {
    status_free_ok,
    status_free_fail,
} status_free;

typedef enum status_cmd {
    cmd_exit,
    cmd_error_alloc,
    cmd_invalid_parameter,
    cmd_sort,
    cmd_rewrite_file,
    cmd_find,
    cmd_print_stud_more_avg,
    cmd_table
} status_cmd;

typedef struct Student {
    unsigned int id;
    char name[256];
    char surname[256];
    char group[256];
    unsigned char* marks;
} Student;

status_cmd command(char** arg_one, char** arg_two);
status_realloc my_realloc(void** var, int size);
status_free free_all(int count, ...);
status_free free_storage(Student* storage, int size);
status_code read_from_file(FILE* in, Student** storage, int* capacity);
int compare_students_group(const void* a, const void* b);
int compare_students_surname(const void* a, const void* b);
int compare_students_name(const void* a, const void* b);
int compare_students_id(const void* a, const void* b);
status_code write_file(const char* out, Student* storage, int size);
bool is_sorted(Student* storage, int size, int (*compare)(const Student*, const Student*));
double get_avg_ball(Student* student);
void print_table(Student* storage, int size);
status_code rewrite_file(FILE* file, Student* storage, int size);
status_code get_student_name(Student* storage, Student** res, int size, const char* find_name);
status_code get_student_surname(Student* storage, Student** res, int size, const char* find_surname);
status_code get_student_id(Student* storage, Student** res, int size, unsigned int find_id);
status_code print_students_group(const char* file_name, char* group_name, Student* storage, int size);
status_code print_student_chars(const char* file_name, Student* stud);
status_code print_stud_bigger_avg(const char* file_name, Student* storage, int size);
#endif