#include "../headers/lab.h"

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return status_free_fail;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
    }

    va_end(ptr);
    return status_free_ok;
}


status_code read_from_file(FILE* in, Student** storage, int* capacity) {
    *capacity = 16;
    (*storage) = (Student*)malloc(sizeof(Student) * (*capacity));
    if (*storage == NULL) {
        return code_error_alloc;
    }
    int index_line = 0;
    int index_storage = 0;
    char* line = NULL;
    bool start = true;
    status_realloc st_realloc;
    int read;
    while ((read = getline(&line, &(size_t){0}, in)) != -1) {
        if (read == -1) {
            free(line);
            return code_error_alloc;
        }
        //line[read - 1] = '\0';
        Student tmp;
        tmp.marks = malloc(sizeof(unsigned char) * 5);
        if (!tmp.marks) {
            free(line);
            return code_error_alloc;
        }
        if (sscanf(line, "%d %s %s %s %c %c %c %c %c", &(tmp.id), tmp.name, tmp.surname, tmp.group, &(tmp.marks[0]), &(tmp.marks[1]), &(tmp.marks[2]), &(tmp.marks[3]), &(tmp.marks[4])) != 9)  {
            free(line);
            return code_invalid_parameter;
        }
        (*storage)[index_storage] = tmp;
        //printf("%d %s %s %s %c %c %c %c %c\n", (*storage)[index_storage].id, (*storage)[index_storage].name, (*storage)[index_storage].surname, (*storage)[index_storage].group, (*storage)[index_storage].marks[0], (*storage)[index_storage].marks[1], (*storage)[index_storage].marks[2], (*storage)[index_storage].marks[3], (*storage)[index_storage].marks[4]);
        index_storage++;
        if (*capacity - 1 == index_storage) {
            *capacity *= 2;
            st_realloc = my_realloc(storage, *capacity);
            if (st_realloc == status_realloc_fail) {
                status_free st_free = free_all(2, line, *storage);
                if (st_free == status_free_fail) {
                    return code_error_alloc;
                }
                return code_error_alloc;
            }
        }
        free(line);
        line = NULL;
        start = true;
    }
    free(line);
    *capacity = index_storage;
    //print_table(*storage, *capacity);
    return code_success;
}

int compare_students(const void* a, const void* b) {
    const double epsilon = 1e-10;
    int diff_id = ((Student *)a)->id - ((Student *)b)->id;
    if (diff_id > 0) {
        return 1;
    } else if (diff_id < 0) {
        return -1;
    }
    int surname_diff = strcmp(((Student *)a)->surname, ((Student *)b)->surname);
    if (surname_diff != 0) {
        return surname_diff;
    }
    int name_diff = strcmp(((Student *)a)->name, ((Student *)b)->name);
    if (name_diff != 0) {
        return name_diff;
    }
    return strcmp(((Student *)a)->group, ((Student *)b)->group);
}

int compare_students_group(const void* a, const void* b) {
    return strcmp(((Student *)a)->group, ((Student *)b)->group);
}

int compare_students_surname(const void* a, const void* b) {
    int surname_diff = strcmp(((Student *)a)->surname, ((Student *)b)->surname);
    return surname_diff;
}

int compare_students_name(const void* a, const void* b) {
    int name_diff = strcmp(((Student *)a)->name, ((Student *)b)->name);
    return name_diff;
}

int compare_students_id(const void* a, const void* b) {
    const double epsilon = 1e-10;
    int diff_id = ((Student *)a)->id - ((Student *)b)->id;
    if (diff_id > 0) {
        return 1;
    } else if (diff_id < 0) {
        return -1;
    }
}

bool is_sorted(Student* storage, int size, int (*compare)(const Student*, const Student*)) {
    for (int i = 0; i < size - 1; i++) {
        int check = compare(&storage[i], &storage[i + 1]);
        if (check > 0) {
            return false;
        }
    }
    return true;
}

double get_avg_ball(Student* student) {
    double sum = 0;
    for (int i = 0; i < 5; i++) {
        int mark = student->marks[i] - '0';
        sum += mark;
        //printf("%lf - %d\n", sum, mark);
    }
    return sum / 5;
}

void print_table(Student* storage, int size) {
    printf("| Surname | Name | id | Group | Avg Ball |\n");
    printf("|----------------------------------------|\n");
    for (int i = 0; i < size; i++) {
        Student tmp = storage[i];
        printf("| %-8s| %-5s| %3d| %-6s| %.2f     |\n", tmp.surname, tmp.name, tmp.id, tmp.group, get_avg_ball(&tmp));

    }
    printf("|----------------------------------------|\n");
}

status_code rewrite_file(FILE* file, Student* storage, int size) {
    for (int i = 0; i < size; i++) {
        Student tmp = storage[i];
        fprintf(file, "%d %s %s %s %c %c %c %c %c\n", tmp.id, tmp.name, tmp.surname, tmp.group, tmp.marks[0], tmp.marks[1], tmp.marks[2], tmp.marks[3], tmp.marks[4]);
    }
    return code_success;
}

status_code get_student_name(Student* storage, Student** res, int size, const char* find_name) {
    int left = 0;
    int right = size - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (strcmp(storage[mid].name, find_name) > 0) {
            right = mid - 1;
        } else if (strcmp(storage[mid].name, find_name) < 0) {
            left = mid + 1;
        } else {
            *res = &storage[mid];
            return code_success;
        }

    }
    *res = NULL;
    return code_success;
}

status_code get_student_surname(Student* storage, Student** res, int size, const char* find_surname) {
    int left = 0;
    int right = size - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (strcmp(storage[mid].surname, find_surname) > 0) {
            right = mid - 1;
        } else if (strcmp(storage[mid].surname, find_surname) < 0) {
            left = mid + 1;
        } else {
            *res = &storage[mid];
            return code_success;
        }

    }
    *res = NULL;
    return code_success;
}

status_code get_student_id(Student* storage, Student** res, int size, unsigned int find_id) {
    int left = 0;
    int right = size - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (storage[mid].id > find_id) {
            right = mid - 1;
        } else if (storage[mid].id < find_id) {
            left = mid + 1;
        } else {
            *res = &storage[mid];
            return code_success;
        }

    }
    *res = NULL;
    return code_success;
}

status_code get_student_group(Student* storage, Student** res, int size, const char* find_group) {
    int left = 0;
    int right = size - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (strcmp(storage[mid].group, find_group) > 0) {
            right = mid - 1;
        } else if (strcmp(storage[mid].group, find_group) < 0) {
            left = mid + 1;
        } else {
            *res = &storage[mid];
            return code_success;
        }

    }
    *res = NULL;
    return code_success;
}


status_code print_student_chars(const char* file_name, Student* stud) {
    FILE* out;
    out = fopen(file_name, "a");
    if (!out) {
        return code_error_oppening;
    }
    double avg_ball = get_avg_ball(stud);
    fprintf(out, "%d %s %s %s %lf\n", stud->id, stud->name, stud->surname, stud->group, avg_ball);
 
    fclose(out);
    return code_success;
}

status_code print_students_group(const char* file_name, char* group_name, Student* storage, int size) {
    FILE* out;
    out = fopen(file_name, "a");
    if (!out) {
        return code_error_oppening;
    }
    for (int i = 0; i < size; i++) {
        Student* tmp = &storage[i];
        if (!strcmp(tmp->group, group_name)) {         
            fprintf(out, "%d %s %s %s %c %c %c %c %c\n", tmp->id, tmp->name, tmp->surname, tmp->group, tmp->marks[0], tmp->marks[1], tmp->marks[2], tmp->marks[3], tmp->marks[4]);
        }
    }
    fclose(out);
    return code_success;
}

status_code print_stud_bigger_avg(const char* file_name, Student* storage, int size) {
    FILE* out;
    out = fopen(file_name, "a");
    if (!out) {
        return code_error_oppening;
    }
    const double epsilon = 1e-10;
    double avg = 0.0;
    for (int i = 0; i < size; i++) {
        avg += get_avg_ball(&storage[i]);
    }
    avg /= size;
    for (int i = 0; i < size; i++) {
        Student* tmp = &storage[i];
        double tmp_avg = get_avg_ball(tmp);
        if (tmp_avg - avg > epsilon) {
            fprintf(out, "%d %s %s %s %c %c %c %c %c\n", tmp->id, tmp->name, tmp->surname, tmp->group, tmp->marks[0], tmp->marks[1], tmp->marks[2], tmp->marks[3], tmp->marks[4]);
        }
    }
    fclose(out);
    return code_success;
}

status_cmd command(char** arg_one, char** arg_two) {
    char* cmd = (char*)malloc(sizeof(char) * STR_SIZE);
    if (!cmd) {
        return cmd_error_alloc;
    }
    int index = 0;
    char symbol = getchar();
    while (!isspace(symbol)) {
        cmd[index++] = symbol;
        symbol = getchar();
    }
    cmd[index] = '\0';
    index = 0;
    if (!strcmp(cmd, "Exit")) {
        free(cmd);
        return cmd_exit;
    } else if (!strcmp(cmd, "Sort")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        return cmd_sort;
    } else if (!strcmp(cmd, "Table")) {
        free(cmd);
        return cmd_table;
    } else if (!strcmp(cmd, "Avg")) {
        free(cmd);
        return cmd_print_stud_more_avg;
    } else if (!strcmp(cmd, "Rewrite")) {
        free(cmd);
        return cmd_rewrite_file;
    } else if (!strcmp(cmd, "Find")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (!isspace(symbol)) {
            (*arg_two)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_two)[index] = '\0';
        index = 0;
        return cmd_find;
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}

status_free free_storage(Student* storage, int size) {
    for (int i = 0; i < size; i++) {
        free(storage[i].marks);
    }
    free(storage);
    return status_free_ok;
}