#include "../headers/lab.h"

status_code create_array(Array** ar, char c) {
    (*ar) = (Array*)malloc(sizeof(Array));
    if (!(*ar)) return code_error_alloc;
    (*ar)->name = toupper(c);
    (*ar)->size = 0;
    (*ar)->capacity = 256;
    (*ar)->data = (int*)malloc(sizeof(int) * (*ar)->capacity);
    if (!(*ar)->data) {
        free(*ar);
        return code_error_alloc;
    }
    //printf("zdesssssaaaqqq\n");
    return code_success;
}

status_code add_to_array(Array** arr, char c, int number) {
    status_code st_act;
    if (!(*arr)) {
        st_act = create_array(arr, c);
        if (st_act != code_success) {
            return st_act;
        }
    }
    (*arr)->data[(*arr)->size] = number;
    (*arr)->size++;
    if ((*arr)->size == (*arr)->capacity - 1) {
        int* tmp = realloc((*arr)->data, sizeof(int) * (*arr)->capacity * 2);
        if (!tmp) {
            return code_error_alloc;
        } else {
            (*arr)->data = tmp;
            (*arr)->capacity *= 2;
        }
    }
    return code_success;
}

status_code remove_from_array(Array* arr, int index, int count) {
    if (count <= 0 || index > arr->size) return code_invalid_parameter;
    int size = arr->size;
    int count_deleted = 0;
    for (int i = index; i < size; i++) {
        if (i + count < size) {
            //printf("here\n");
            arr->data[i] = arr->data[i + count];
        }
    }
    int* tmp = NULL;
    if (count < size) {
        tmp = realloc(arr->data, sizeof(int) * (size - count));
        arr->size -= count;
    } else {
        tmp = realloc(arr->data, sizeof(int) * index);
        arr->size = index;
    }
    if (!tmp) {
        return code_error_alloc;
    } else {
        arr->data = tmp;
    }
    return code_success;
}

status_code save_array(Array* arr, const char* filename) {
    if (!arr) return code_invalid_parameter;
    FILE* file = fopen(filename, "w");
    if (!file) {
        return code_error_oppening;
    }
    for (int i = 0; i < arr->size; i++) {
        fprintf(file, "%d ", arr->data[i]);
    }
    fclose(file);
    return code_success;
}

void free_array(Array* arr) {
    if (!arr) return;
    free(arr->data);
    free(arr);
}

status_code load_array(Array** arr, char name, const char* filename) {
    status_code st_act;
    if (!(*arr)) {
        //printf("blyaha muha\n");
        st_act = create_array(arr, name);
        if (st_act != code_success) return st_act;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        return code_error_oppening;
    }
    //printf("otkril\n");
    int number;
    while (fscanf(file, "%d", &number) == 1) {
        //printf("%d>>>\n", number);
        st_act = add_to_array(arr, name, number);
        if (st_act != code_success) {
            free_array(*arr);
            return st_act;
        }
    }
    fclose(file);
    return code_success;
}

status_code concat_arrays(Array* a, Array* b) {
    if (!b || b->size == 0) return code_success;
    int size = b->size;
    status_code st_act;
    for (int i = 0; i < size; i++) {
        st_act = add_to_array(&a, a->name, b->data[i]);
        if (st_act != code_success) {
            free_array(a);
            free_array(b);
            return st_act;
        }
    }
    return code_success;
}

void stats_array(Array* a) {
    if (!a) return;
    printf("Array %c\n", a->name);
    printf("Size: %d\n", a->size);
    if (a->size == 0) return;
    int size = a->size;
    int max_count = 0;
    int most_common = a->data[0];
    for (int i = 1; i < size; i++) {
        int counter = 1;
        for (int j = i + 1; j < size; j++) {
            if (a->data[i] == a->data[j]) {
                counter++;
            }
            if (counter > max_count) {
                max_count = counter;
                most_common = a->data[i];
            } else if (counter == max_count) {
                most_common = fmax(most_common, a->data[i]);
            }
        }
    }
    printf("Mode: %d\n", most_common);
    int sum = a->data[0]; 
    int max_elem = a->data[0]; 
    int min_elem = a->data[0]; 
    int index_max = 0;
    int index_min = 0;
    for (int i = 1; i < size; i++) {
        sum += a->data[i];
        if (a->data[i] > max_elem) {
            max_elem = a->data[i];
            index_max = i;
        }
        if (a->data[i] < min_elem) {
            min_elem = a->data[i];
            index_min = i;
        }
    }
    double mean = sum /= size;
    printf("Max: %d, index: %d\n", max_elem, index_max);
    printf("Min: %d, index: %d\n", min_elem, index_min);
    printf("Mean: %lf\n", mean);
    double max_sigma = 0;
    for (int i = 0; i < size; i++) {
        double cur = fabs(a->data[i] - max_sigma);
        if (cur - max_sigma > 1e-8) {
            max_sigma = cur;
        }
    }
    printf("Sigma: %lf\n", max_sigma);
    printf("\n");
}

Array* find_array(Array** storage, int size, char name) {
    if (!storage) return NULL;
    int left = 0;
    int right = size - 1;
    while (left <= right) {
        int middle = (left + right) / 2;
        char cur = storage[middle]->name; 
        if (cur < name) {
            left = middle + 1;
        } else if (cur > name) {
            right = middle - 1;
        } else {
            return storage[middle];
        }
    }
    return NULL;
}

status_code Free(Array* arr) {
    if (!arr) return code_success;
    free(arr->data);
    arr->data = (int*)malloc(sizeof(int) * 256);
    if (!arr->data) return code_error_alloc;
    arr->size = 0;
    return code_success;
}

void print(Array* arr, char name, char* mode) {
    if (!arr) return;
    if (!strcmp(mode, "all")) {
        //printf("pechataem vse\n");
        for (int i = 0; i < arr->size; i++) {
            printf("%d ", arr->data[i]);
        }
        printf("\n");
    } else {
        int left, right;
        int read = sscanf(mode, "%d %d", &left, &right);
        if (read != 2) {
            int read = sscanf(mode, "%d", &left);
            //printf("chisla: %d\n", left);
            if (read != 1) return;
            printf("%d ", arr->data[left]);
        } else {
            if (left > right || left > arr->size || right > arr->size) {
                return;
            }
            //printf("chisla: %d %d\n", left, right);
            for (int i = left; i <= right; i++) {
                printf("%d ", arr->data[i]);
            }
            printf("\n");
        }
    }
}

status_code copy_arrays(Array* a, Array** b, int start, int end) {
    status_code st_act;
    if (!(*b)) return code_invalid_parameter;
    if (start > a->size) return code_invalid_parameter;
    int upper_bound = fmin(end, a->size);
    for (int i = start; i < upper_bound; i++) {
        st_act = add_to_array(b, (*b)->name, a->data[i]);
        if (st_act != code_success) return st_act;
    }
    return code_success;
}

int compare_ascending(const void* a, const void* b) {
    return (*(int *)a - *(int *)b);
}

int compare_descending(const void* a, const void* b) {
    return (*(int *)b - *(int *)a);
}

int compare_random(const void* a, const void* b) {
    return rand() % 3 - 1;
}

int get_my_rand(int min, int max) {
    return min + rand() % (max - min + 1);
}

status_code rand_fill_array(Array** arr, char name, char* arg) {
    if (!arg || !strcmp(arg, "\0")) return code_invalid_parameter;
    status_code st_act;
    if (!(*arr)) {
        st_act = create_array(arr, name);
        if (st_act != code_success) return st_act;
    } 
    int count, lb, rb;
    int read = sscanf(arg, "%d %d %d", &count, &lb, &rb);
    if (read != 3) {
        free_array(*arr);
        return code_invalid_parameter;
    }
    for (int i = 0; i < count; i++) {
        int number = get_my_rand(lb, rb);
        st_act = add_to_array(arr, name, number);
        if (st_act != code_success) {
            free_array(*arr);
            return code_invalid_parameter;
        }
    }
    return code_success;
}

void free_storage(Array** st, int size) {
    if (!st) return;

    for (int i = 0; i < size; i++) {
        free_array(st[i]);
    }
    free(st);
}

void sort(Array* a, char mode) {
    if (!a) return;
    if (mode == '+') {
        qsort(a->data, a->size, sizeof(int), compare_ascending);
    } else if (mode == '-') {
        qsort(a->data, a->size, sizeof(int), compare_descending);
    } else {
        return;
    }
}

void Shuffle(Array* a) {
    if (!a) return;
    qsort(a->data, a->size, sizeof(int), compare_random);
}

void add_to_storage(Array** st, Array* a, int* capacity) {
    if (!a) return;
    if (*capacity == 0) {
        st[*capacity] = a;
        (*capacity)++;
    } else {
        int index = 0;
        while (index < *capacity && a->name > st[index]->name) {
            index++;
        }
        for (int i = *capacity; i > index; i--) {
            st[i] = st[i - 1];
        }
        st[index] = a;
        (*capacity)++;
    }
}

status_cmd command(char* input, Array** storage, int* size_storage) {
    if (!input || !strcmp(input, "\0")) return cmd_invalid_parameter;
    char* check = strchr(input, ';');
    if (check == NULL) {
        return cmd_invalid_parameter;
    }
    //printf("size storage: %d\n", *size_storage);
    int size = strlen(input);
    //printf("\t%s, size = %d\n", input, size);
    int index = 0;
    char* cmd = (char*)malloc(sizeof(char) * size);
    if (!cmd) return cmd_error_alloc;
    char* arg = (char*)malloc(sizeof(char) * size);
    if (!arg) {
        free(cmd);
        cmd = NULL;
        return cmd_error_alloc;
    }
    bool is_read_cmd = false;
    for (int i = 0; i < size; i++) {
        char symb = input[i];
        //printf("dymb %c %d %d\n", symb, is_read_cmd, index);
        if (!isspace(symb)) {
            if (symb == ',' || symb == ';') continue;
            if (!is_read_cmd) {
                cmd[index] = symb;
                index++;
            } else {
                arg[index] = symb;
                index++;
            }
        }
        if (isspace(symb)) {
            if (is_read_cmd) {
                arg[index] = symb;
                index++;
            }
            if (index != 0 && !is_read_cmd) {
                cmd[index] = '\0';
                index = 0;
                is_read_cmd = true;
            }
        }
    }
    if (is_read_cmd) {
        arg[index] = '\0';
        index = 0;
    } else {
        cmd[index] = '\0';
        index = 0;
    }
    status_code st_act;
    char name_array_first, name_array_second;
    int size_arg = strlen(arg);
    Array* tmp = NULL;
    bool need_to_add = false;
    //printf("%s\n %s\n", cmd, arg);
    if (!strcmp(cmd, "Load")) 
    {
        char* filename = (char*)malloc(sizeof(char) * size);
        if (!filename) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        //printf("zdes\n");
        name_array_first = arg[0];
        for (int i = 2; i < size_arg; i++) {
            filename[index] = arg[i];
            index++;
        }
        filename[index] = '\0';
        //printf("%s file\n", filename);
        index = 0;
        tmp = find_array(storage, *size_storage, name_array_first);
        if (tmp == NULL) need_to_add = true;
        //printf("zdesfdsasdf\n");
        st_act = load_array(&tmp, name_array_first, filename);
        if (st_act != code_success) {
            //printf("su4ara\n");
            free(filename);
            free(cmd);
            free(arg);
            if (st_act == code_error_oppening) {
                return cmd_error_oppening;
            } else {
                return cmd_invalid_parameter;
            }
        }
        free(filename);
        //printf("zaebis dobavlenie\n");
        //stats_array(tmp);
        if (need_to_add) {
            add_to_storage(storage, tmp, size_storage);
        }
    } 
    else if (!strcmp(cmd, "Save")) 
    {
        char* filename = (char*)malloc(sizeof(char) * size);
        if (!filename) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        name_array_first = arg[0];
        for (int i = 2; i < size_arg; i++) {
            filename[index] = arg[i];
            index++;
        }
        filename[index] = '\0';
        index = 0;
        tmp = find_array(storage, *size_storage, name_array_first);
        st_act = save_array(tmp, filename);
        if (st_act != code_success) {
            free(filename);
            free(cmd);
            free(arg);
            if (st_act == code_error_oppening) {
                return cmd_error_oppening;
            } else {
                return cmd_invalid_parameter;
            }
        }
        free(filename);
    } 
    else if (!strcmp(cmd, "Rand")) 
    {
        char* arg_two = (char*)malloc(sizeof(char) * size);
        if (!arg_two) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        name_array_first = arg[0];
        for (int i = 2; i < size_arg; i++) {
            arg_two[index] = arg[i];
            index++;
        }
        arg_two[index] = '\0';
        index = 0;
        tmp = find_array(storage, *size_storage, name_array_first);
        if (tmp == NULL) need_to_add = true;
        st_act = rand_fill_array(&tmp, name_array_first, arg_two);
        if (st_act != code_success) {
            free(arg_two);
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
        free(arg_two);
        if (need_to_add) {
            add_to_storage(storage, tmp, size_storage);
        }
    } 
    else if (!strcmp(cmd, "Concat")) 
    {
        int read = sscanf(arg, "%c %c", &name_array_first, &name_array_second);
        if (read != 2) {
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
        tmp = find_array(storage, *size_storage, name_array_first);
        Array* tmp2 = NULL;
        tmp2 = find_array(storage, *size_storage, name_array_second);
        st_act = concat_arrays(tmp, tmp2);
        if (st_act != code_success) {
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
    } 
    else if (!strncmp(cmd, "Free", 4)) 
    {
        int size_cmd = strlen(cmd);
        printf("----%s\n", cmd);
        name_array_first = cmd[5];
        printf("bukva %c\n", name_array_first);
        tmp = find_array(storage, *size_storage, name_array_first);
        st_act = Free(tmp);
        if (st_act != code_success) {
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
    } 
    else if (!strcmp(cmd, "Remove")) 
    {
        char* arg_two = (char*)malloc(sizeof(char) * size);
        if (!arg_two) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        name_array_first = arg[0];
        for (int i = 2; i < size_arg; i++) {
            arg_two[index] = arg[i];
            index++;
        }
        arg_two[index] = '\0';
        index = 0;
        int index_rem_st, index_rem_end;
        int read = sscanf(arg_two, "%d %d", &index_rem_st, &index_rem_end);
        if (read != 2) {
            free(cmd);
            free(arg);
            free(arg_two);
            return cmd_invalid_parameter;
        }
        tmp = find_array(storage, *size_storage, name_array_first);
        st_act = remove_from_array(tmp, index_rem_st, index_rem_end);
        if (st_act != code_success) {
            free(arg_two);
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
        free(arg_two);
    } 
    else if (!strcmp(cmd, "Copy")) 
    {
        char* arg_two = (char*)malloc(sizeof(char) * size);
        if (!arg_two) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        name_array_first = arg[0];
        for (int i = 2; i < size_arg; i++) {
            arg_two[index] = arg[i];
            index++;
        }
        arg_two[index] = '\0';
        index = 0;
        int index_rem_st, index_rem_end;
        printf("doshel\n");
        int read = sscanf(arg_two, "%d %d %c", &index_rem_st, &index_rem_end, &name_array_second);
        if (read != 3) {
            free(cmd);
            free(arg);
            free(arg_two);
            return cmd_invalid_parameter;
        }
        printf("doshel2\n");
        tmp = find_array(storage, *size_storage, name_array_first);
        Array* tmp2 = NULL;
        tmp2 = find_array(storage, *size_storage, name_array_second);
        if (tmp2 == NULL) need_to_add = true;
        st_act = copy_arrays(tmp, &tmp2, index_rem_st, index_rem_end);
        if (st_act != code_success) {
            free(arg_two);
            free(cmd);
            free(arg);
            return cmd_invalid_parameter;
        }
        free(arg_two);
        if (need_to_add) {
            add_to_storage(storage, tmp2, size_storage);
        }
    } 
    else if (!strcmp(cmd, "Sort")) 
    {
        name_array_first = arg[0];
        tmp = find_array(storage, *size_storage, name_array_first);
        sort(tmp, arg[1]);
    } 
    else if (!strcmp(cmd, "Shuffle")) 
    {
        name_array_first = arg[0];
        tmp = find_array(storage, *size_storage, name_array_first);
        Shuffle(tmp);
    } 
    else if (!strcmp(cmd, "Stats")) 
    {
        name_array_first = arg[0];
        tmp = find_array(storage, *size_storage, name_array_first);
        stats_array(tmp);
    } 
    else if (!strcmp(cmd, "Print")) 
    {
        
        name_array_first = arg[0];
        tmp = find_array(storage, *size_storage, name_array_first);
        char* arg_two = (char*)malloc(sizeof(char) * size);
        if (!arg_two) {
            free(cmd);
            free(arg);
            return cmd_error_alloc;
        }
        for (int i = 2; i < size_arg; i++) {
            arg_two[index] = arg[i];
            index++;
        }
        arg_two[index] = '\0';
        print(tmp, name_array_first, arg_two);
        free(arg_two);
    } 
    else if (!strcmp(cmd, "Exit")) 
    {
        free(cmd);
        free(arg);
        return cmd_exit;
    } 
    free(cmd);
    free(arg);
    return cmd_success;
}