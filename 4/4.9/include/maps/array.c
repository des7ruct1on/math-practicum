#include "headers/array.h"

status_code create_array(Array** arr, int _capacity) {
    (*arr) = (Array*)malloc(sizeof(Array));
    if (!*arr) return code_error_alloc;
    printf("tut1\n");
    (*arr)->capacity = _capacity;
    (*arr)->size = 0;
    printf("tut2\n");
    (*arr)->keys = (char**)malloc(_capacity * sizeof(char*));
    printf("tut3\n");
    if (!(*arr)->keys) {
        free(*arr);
        return code_error_alloc;
    }
    printf("tut\n");
    (*arr)->posts = (Post**)malloc(_capacity * sizeof(Post*));
    if (!(*arr)->posts) {
        free((*arr)->keys);
        free(*arr);
        return code_error_alloc;
    }

    (*arr)->min_load = NULL;
    printf("tut\n");
    return code_success;
}

status_code insert_array(Logger* logger, Array* arr, char* key, Post* office) {
    create_log(&logger, "started inserting\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
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
    create_log(&logger, "finished inserting\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
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
            return arr->posts[mid];
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

char* find_post_array(Array* arr, Post* find) {
    for (int i = 0; i < arr->size; i++) {
        if (arr->posts[i] == find) {
            return arr->keys[i];
        }
    }
    return NULL;
}