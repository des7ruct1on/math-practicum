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
        Student tmp;
        if (!sscanf(line, "%d %s %s %s %c %c %c %c %c", &(tmp.id), tmp.name, tmp.surname, tmp.group, tmp.marks[0], tmp.marks[1], tmp.marks[2], tmp.marks[3], tmp.marks[4]))  {
            free(line);
            return code_invalid_parameter;
        }
        (*storage)[index_storage] = tmp;
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

bool is_sorted(Student* storage, int size) {
    for (int i = 0; i < size - 1; i++) {
        int check = compare_students(&storage[i], &storage[i + 1]);
        if (check < 0) {
            return false;
        }
    }
    return true;
}

double get_avg_ball(Student* student) {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += student->marks[i];
    }
    return (double)sum / 5.0;
}

void print_table(Student* storage, int size) {
    printf("| Surname | Name | id | Group | Avg Ball |\n");
    for (int i = 0; i < size; i++) {
        Student tmp = storage[i];
        printf("| %-8s| %-5s| %3d| %-6s| %.2f     |\n", tmp.surname, tmp.name, tmp.id, tmp.group, get_avg_ball(&tmp));

    }
    printf("|----------------------------------------|\n");
}

void rewrite_file(FILE* out, Student* storage, int size) {
    for (int i = 0; i < size; i++) {
        Student tmp = storage[i];
        fprintf(out, "%d %s %s %s %c %c %c %c %c", tmp.id, tmp.name, tmp.surname, tmp.group, tmp.marks[0], tmp.marks[1], tmp.marks[2], tmp.marks[3], tmp.marks[4]);
    }
}

Student* get_student_name(Student* storage, int size, int* index, const char* find_name) {
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
            *index = mid;
            return &storage[mid];
        }

    }
    return NULL;
}

Student* get_student_surname(Student* storage, int size, int* index, const char* find_surname) {
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
            *index = mid;
            return &storage[mid];
        }

    }
    return NULL;
}

Student* get_student_id(Student* storage, int size, int* index, unsigned int find_id) {
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
            *index = mid;
            return &storage[mid];
        }

    }
    return NULL;
}

Student* get_student_group(Student* storage, int size, int* index, const char* find_group) {
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
            *index = mid;
            return &storage[mid];
        }

    }
    return NULL;
}


void print_student_chars(FILE* out, Student* stud) {
    double avg_ball = get_avg_ball(stud);
    fprintf(out, "%d %s %s %s %lf\n", stud->id, stud->name, stud->surname, stud->group, avg_ball);
}

void print_stud_bigger_avg(FILE* out, Student* storage, int size) {
    const double epsilon = 1e-10;
    double avg = 0.0;
    for (int i = 0; i < size; i++) {
        avg += get_avg_ball(&storage[i]);
    }
    avg /= size;
    for (int i = 0; i < size; i++) {
        Student* tmp = &storage[i];
        double tmp_avg = get_avg_ball(tmp);
        if (fabs(tmp_avg - avg) > epsilon) {
            print_student_chars(out, tmp);
        }
    }
}

