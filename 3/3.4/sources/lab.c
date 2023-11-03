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


My_string* String(unsigned int _size) {
    My_string* new = malloc(sizeof(My_string));
    new->data = malloc(sizeof(char) * (_size + 1));
    new->size = _size;
    return new;
}



void String_clear(My_string* str) {
    
}



status_code my_strcpy(My_string* destination, const My_string* source);



My_string* my_strcpy_new(const My_string* source);



int my_strcmp(My_string* left, My_string* right);
