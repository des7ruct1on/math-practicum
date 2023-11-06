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

status_code create_adress(Adress** new_adress, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    *new_adress = (Adress*)malloc(sizeof(Adress));
    if (*new_adress == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    char* tmp = malloc(sizeof(char) * _size);
    if (!tmp) {
        return code_error_alloc;
    }
    int check;
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_adress)->city = String(tmp);
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_adress)->street = String(tmp);
    if ((check = sscanf(info->data, "%d ", &(*new_adress)->house)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_adress)->block = String(tmp);
    if ((check = sscanf(info->data, "%d ", &(*new_adress)->flat)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_adress)->index = String(tmp);
    free(tmp);
    return code_success;
}

status_code create_mail(Mail** new_mail, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    (*new_mail) = (Mail*)malloc(sizeof(Mail));
    if (*new_mail == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    char* tmp = malloc(sizeof(char) * _size);
    if (!tmp) {
        return code_error_alloc;
    }
    int check;
    Adress* new_ad = NULL;
    status_code st_adress = create_adress(&new_ad, info);
    if (st_adress == NULL) {
        free(*new_mail);
        free(tmp);
        return code_error_alloc;
    }
    (*new_mail)->ad = *new_ad; 
    if ((check = sscanf(info->data, "%lf ", &(*new_mail)->weight)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_mail)->id = String(tmp);
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_invalid_parameter;
    }
    (*new_mail)->time_create = String(tmp);
    if ((check = sscanf(info->data, "%s ", tmp)) != 1) {
        free(tmp);
        return code_success;
    }
    (*new_mail)->time_get = String(tmp);
    free(tmp);
    return code_success;

}

status_code create_post(Post** new_post, int _capacity, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    (*new_post) = (Post*)malloc(sizeof(Post));
    if (*new_post == NULL) {
        return code_error_alloc;
    }
    Adress* new_ad = NULL;
    status_code st_adress = create_adress(&new_ad, info);
    if (st_adress == code_error_alloc) {
        return code_error_alloc;
    }
    (*new_post)->size = 0;
    (*new_post)->capacity = _capacity;
    (*new_post)->mails = (Mail*)malloc(sizeof(Mail*) * _capacity);
    if (!(*new_post)->mails) {
        free(*new_post);
        return code_error_alloc;
    }
    return code_success;
}

status_code add_mail(Post** cur_post, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    Mail* new_mail = NULL;
    status_code st_mail = create_mail(&new_mail, info);
    if (st_mail == code_error_alloc) {
        return code_error_alloc;
    }
    int index = (*cur_post)->size;
    (*cur_post)->mails[index] = *new_mail;
    (*cur_post)->size++;
    if ((*cur_post)->size == (*cur_post)->capacity) {
        (*cur_post)->capacity *= 2;
        status_realloc st_realloc = my_realloc(cur_post, (*cur_post)->capacity);
        if (st_realloc == status_realloc_fail) {
            free(st_mail);
            return code_error_alloc;
        }
    }
    return code_success;
}

bool check_equal(Adress* left, Adress* right) {
    return is_equal(left->city, right->city) && is_equal(left->street, right->street) && is_equal(left->block, right->block) && is_equal(left->index, right->index) && left->house == right->house && left->flat == right->flat;
}

int free_adress(Adress* tmp) {
    if (!tmp) {
        return 0;
    }
    status_free st_free = free_all(4, tmp->city, tmp->street, tmp->block, tmp->index);
    if (st_free == status_free_fail) {
        return -1;
    }
    free(tmp);
    return 1;
}

int free_mail(Mail* tmp) {
    if (!tmp) {
        return 0;
    }
    status_free st_free = free_all(3, tmp->id, tmp->time_create, tmp->time_get);
    if (st_free == status_free_fail) {
        return -1;
    }
    int check = free_adress(&tmp->ad);
    free(tmp);
    return check + 1;
}


Post* find_post(Post* posts, const My_string* id) {
    if (!id->size) {
        return NULL;
    }
    const double epsilon = 1e-8;
    Adress* check = NULL;
    status_code st_adress = create_adress(&check, id);
    if (st_adress == code_error_alloc) {
        return NULL;
    }
    int _size = posts->size;
    Post* current;
    for (int i = 0; i < _size; i++) {
        current = &posts[i];
        if (check_equal(check, current->cur_id)) {
            free(check);
            return current;
        }
    }
    return NULL;
}

void get_date(My_string* tmp, int* day, int* month, int* year, int* hour, int* minute, int* seconds) {
    if (tmp) {
        return;
    }
    sscanf(tmp->data, "%d:%d:%d %d:%d:%d", &day, &month, &year, &hour, &minute, &seconds);
}

Mail* find_mail(Post* _mails, My_string id, int size) {
    if (!_mails) {
        return NULL;
    }
    int left_ind = 0;
    int right_ind = size - 1;
    int mid_ind = left_ind + right_ind;
    while (left_ind <= right_ind) {
        if (my_strcmp(&id, _mails->mails[mid_ind].id) < 0) {
            left_ind = mid_ind + 1;
        } else if (my_strcmp(&id, _mails->mails[mid_ind].id) > 0) {
            right_ind = mid_ind - 1;
        } else {
            return &_mails->mails[mid_ind];
        }
    }
    return NULL;
}

int compare_mails(const void* a, const void* b) {
    const double epsilon = 1e-10;
    int diff_id = my_strcmp(((Mail*)a)->ad.index, ((Mail*)b)->ad.index);
    if (diff_id > 0) {
        return 1;
    } else if (diff_id < 0) {
        return -1;
    }
    return my_strcmp(((Mail*)a)->id, ((Mail*)b)->id);
}

void get_date_now(int* day, int* month, int* year, int* hour, int* minute, int* seconds) {
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    *day = timeinfo->tm_mday;
    *month = timeinfo->tm_mon + 1; 
    *year = timeinfo->tm_year + 1900; 
    *hour = timeinfo->tm_hour;
    *minute = timeinfo->tm_min;
    *seconds = timeinfo->tm_sec;
}

