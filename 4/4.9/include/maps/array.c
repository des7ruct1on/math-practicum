#include "headers/array.h"

status_code create_array(Array** arr, int _capacity) {
    (*arr) = (Array*)malloc(sizeof(Array));
    if (!*arr) return code_error_alloc;
    (*arr)->capacity = _capacity;
    (*arr)->size = 0;
    (*arr)->keys = (char**)malloc(_capacity * sizeof(char*));
    if ((*arr)->keys) {
        free(*arr);
        return code_error_alloc;
    }
    (*arr)->posts = (Post**)malloc(_capacity * sizeof(Post*));
    if (!(*arr)->posts) {
        free((*arr)->keys);
        free(*arr);
        return code_error_alloc;
    }

    (*arr)->min_load = NULL;
    return code_success;
}

status_code insert_array(Array* arr, char* key, Post* office) {
    if (!key || !office) return code_invalid_parameter;
    int index = arr->size;
    while (index && strcmp(key, arr->keys[index - 1]) < 0) {
        arr->keys[index] = arr->keys[index - 1];
        arr->posts[index] = arr->posts[index - 1];
        index--;
    }
    arr->keys[index] = key;
    arr->posts[index] = office;
    arr->size++;

    if (arr->size == arr->capacity - 1) {
        arr->capacity *= 2;
        arr->keys = (char**)malloc(arr->capacity * sizeof(char*));
        if (arr->keys) {
            arr->capacity /= 2;
            free(arr);
            return code_error_alloc;
        }
        arr->posts = (Post**)malloc(arr->capacity * sizeof(Post*));
        if (!arr->posts) {
            arr->capacity /= 2;
            free(arr->keys);
            free(arr);
            return code_error_alloc;
        }

    }
    return code_success;
}

Post* search_array(Array* arr, char* key) {
    int left = 0;
    int right = arr->size - 1;
    int mid;
    while (left <= right) {
        mid = (left + right) / 2;
        if (strcmp(key, arr->keys[mid]) < 0) {
            right = mid - 1;
        } else 
        if (strcmp(key, arr->keys[mid]) > 0) {
            left = mid + 1;
        } else {
            return arr->keys[mid];
        }
    }
    return NULL;
}

void free_array(Array* arr, void(*free_storage)(void*)) {
    if (!arr) return;
    for (int i = 0; i < arr->size; i++) {
        free(arr->keys[i]);
        free_storage(arr->posts[i]->storage);
        free(arr->posts[i]->ops);
    }
    free(arr->keys);
    free(arr->posts);
    free(arr);
}