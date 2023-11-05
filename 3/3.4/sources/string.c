#include "../headers/string.h"

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

int my_strlen(const My_string* str) {
    int len = 0;
    while(str->data[len] != '\0') len++;
    return len;
}



My_string* String(const char* str) {
    int _size = strlen(str);
    My_string* new = malloc(sizeof(My_string));
    new->data = malloc(sizeof(char) * (_size + 1));
    if (str) {
        for (int i = 0; i < _size; i++) {
            new->data[i] = str[i];
        }
        new->data[_size] = '\0';
    }
    new->size = _size;
    return new;
}



void string_clear(My_string* str) {
    if (!str) {
        return;
    }
    free(str->data);
    str->data = NULL;
    str->size = 0;
}



int my_strcpy(My_string** destination, const My_string* source) {
    if (!source) {
        return 1;
    }
    if (*destination == NULL) {
        *destination = String(source->data);
        return source->size;
    }
    int size_dest = (*destination)->size;
    int size_source = source->size;
    if ((*destination)->data) {
        string_clear(*destination);
    }
    (*destination) = String(source->data);
    return size_source;
}



My_string* my_strcpy_new(const My_string* source) {
    if (!source) {
        return NULL;
    }
    int _size = source->size;
    My_string* new = malloc(sizeof(My_string));
    new->data = malloc(sizeof(char) * (_size + 1));
    if (source->data) {
        for (int i = 0; i < _size; i++) {
            new->data[i] = source->data[i];
        }
        new->data[_size] = '\0';
    }
    new->size = _size;
    return new;
}



int my_strcmp(My_string* left, My_string* right) {
    int len_comp = left->size - right->size;
    if (!len_comp) {
        int i = 0;
        while (left->data[i] != '\0' && left->data[i] == right->data[i]) {
            i++;
        }
        return left->data[i] - right->data[i];
    }
    return len_comp;
}

int is_equal(My_string* left, My_string* right) {
    int i = 0;
    while (left->data[i] != '\0' && left->data[i] == right->data[i]) {
        i++;
    }
    return left->data[i] == right->data[i] && right->size == left->size;
}

int concat_strings(My_string** left, const My_string* right) {
    if (!right->size) {
        return 1;
    }

    int new_size = (*left)->size + right->size;
    status_realloc st_realloc = my_realloc(&(*left)->data, new_size + 1);

    if (st_realloc == status_realloc_fail) {
        return -1;
    }

    int left_index = (*left)->size;
    int right_index = 0;

    while (right_index < right->size) {
        (*left)->data[left_index++] = right->data[right_index++];
    }

    (*left)->data[new_size] = '\0';
    (*left)->size = new_size;

    return 1;
}

void print_str(My_string* str) {
    if (!str) {
        return;
    }
    printf("%s\n", str->data);
}