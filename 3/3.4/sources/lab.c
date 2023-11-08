#include "../headers/lab.h"
#include "../headers/string.h"

status_code create_adress(Adress** new_adress, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }
    *new_adress = (Adress*)malloc(sizeof(Adress));
    if (*new_adress == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    char* _city = malloc(sizeof(char) * _size);
    if (!_city) {
        return code_error_alloc;
    }
    char* _street = malloc(sizeof(char) * _size);
    if (!_street) {
        return code_error_alloc;
    }
    char* _block = malloc(sizeof(char) * _size);
    if (!_block) {
        return code_error_alloc;
    }
    char* _index = malloc(sizeof(char) * _size);
    if (!_index) {
        return code_error_alloc;
    }
    int check;
    check = sscanf(info->data ,"%s %s %d %s %d %s", _city, _street, &(*new_adress)->house, _block, &(*new_adress)->flat, _index);
    if (check != 6) {
        return code_invalid_parameter;
    }
    (*new_adress)->city = String(_city);
    (*new_adress)->street = String(_street);
    (*new_adress)->block = String(_block);
    (*new_adress)->index = String(_index);
    status_free st_free;
    st_free = free_all(4, _city, _street, _block, _index);
    if (st_free == status_free_fail) {
        return code_error_alloc;
    }
    //printf("        %s %s %d %s %d %s\n", _city, _street, (*new_adress)->house, _block, (*new_adress)->flat, _index);
    return code_success;
}

status_code create_mail(Mail** new_mail, const My_string* info) {
    if (!info->size) {
        return code_invalid_parameter;
    }

    *new_mail = (Mail*)malloc(sizeof(Mail));
    if (*new_mail == NULL) {
        return code_error_alloc;
    }
    int _size = info->size;
    int check;
    Adress* new_ad = NULL;
    status_free st_free;
    status_code st_adress = create_adress(&new_ad, info);
    if (st_adress == code_error_alloc) {
        free(*new_mail);
        return code_error_alloc;
    }
    (*new_mail)->ad = *new_ad;
    //printf("        %s %s %d %s %d %s\n", (*new_mail)->ad.city->data, (*new_mail)->ad.street->data, (*new_mail)->ad.house, (*new_mail)->ad.block->data, (*new_mail)->ad.flat, (*new_mail)->ad.index->data);
    char* _id = malloc(sizeof(char) * _size); 
    if (!_id) {
        free(*new_mail);
        return code_error_alloc;
    }
    char* information = strdup(info->data);
    for (int i = 0; i < 6; ++i) {
        sscanf(information, "%s", _id);
        information += strlen(_id) + 1;
    }
    char* _time_create_date = malloc(sizeof(char) * _size);  
    if (!_time_create_date) {
        free(*new_mail);
        return code_error_alloc;
    }
    char* _time_create_time = malloc(sizeof(char) * _size);  
    if (!_time_create_date) {
        free(*new_mail);
        return code_error_alloc;
    }
    char* _time_get_date = malloc(sizeof(char) * _size);  
    if (!_time_get_date) {
        free(*new_mail);
        return code_error_alloc;
    }
    char* _time_get_time = malloc(sizeof(char) * _size);  
    if (!_time_get_time) {
        free(*new_mail);
        return code_error_alloc;
    }
    sscanf(information, "%lf %s %s %s %s %s", &(*new_mail)->weight, _id, _time_create_date, _time_create_time, _time_get_date, _time_get_time);
    (*new_mail)->id = String(_id);
    My_string* time_cr_date = String(_time_create_date);
    My_string* time_cr_time = String(_time_create_time);
    My_string* time_get_date = String(_time_get_date);
    My_string* time_get_time = String(_time_get_time);
    My_string* null_string = String(" ");
    concat_strings(&time_cr_date, null_string);
    concat_strings(&time_cr_date, time_cr_time);
    concat_strings(&time_get_date, null_string);
    concat_strings(&time_get_date, time_get_time);
    (*new_mail)->time_create = time_cr_date;
    (*new_mail)->time_get = time_get_date;
    string_clear(time_cr_time);
    string_clear(null_string);
    string_clear(time_get_time);
    st_free = free_all(3, time_get_time, null_string, time_cr_time);
    return code_success;
}


status_code create_post(Post** new_post, const My_string* info) {
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
    (*new_post)->capacity = 32;
    (*new_post)->mails = (Mail*)malloc(sizeof(Mail) * 32);
    if (!(*new_post)->mails) {
        free(*new_post);
        return code_error_alloc;
    }
    (*new_post)->cur_id = new_ad;
    return code_success;
}

status_code add_post_storage(Post*** new_post, Post* new, int* capacity, int* size) {
    if (!new) {
        return code_invalid_parameter;
    }
    /*
    if (*new_post == NULL) {
        (*new_post) = malloc(sizeof(Post*) * (*capacity));
        if (*new_post == NULL) {
            return code_error_alloc;
        }    
    }
    */
    print_str(new->cur_id->index);
    (*new_post)[*size] = new;
    (*size)++;
    for (int i = 0; i < *size; i++) {
        print_str((*new_post)[i]->cur_id->city);
    }
    status_realloc st_realloc;
    if (*size == *capacity - 1) {
        *capacity *= 2;
        st_realloc = my_realloc(new_post, sizeof(Post*) * (*capacity));
        if (st_realloc == status_realloc_fail) {
            return code_error_alloc;
        }
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
    ((*cur_post)->size)++;
    if ((*cur_post)->size == (*cur_post)->capacity) {
        (*cur_post)->capacity *= 2;
        status_realloc st_realloc = my_realloc(&(*cur_post)->mails, sizeof(Mail) * (*cur_post)->capacity);
        if (st_realloc == status_realloc_fail) {
            free(new_mail);
            return code_error_alloc;
        }
    }

    return code_success;
}

status_code remove_mail(Post* cur_post, const My_string* id) {
    if (!cur_post || !id || !id->size) {
        return code_invalid_parameter;
    }

    int index_to_remove = -1;
    for (int i = 0; i < cur_post->size; i++) {
        if (is_equal(cur_post->mails[i].id, id)) {
            index_to_remove = i;
            break;
        }
    }

    if (index_to_remove == -1) {
        return code_invalid_parameter;
    }

    for (int i = index_to_remove; i < cur_post->size - 1; i++) {
        cur_post->mails[i] = cur_post->mails[i + 1];
    }

    cur_post->size--;
    return code_success;
}


bool check_equal(Adress* left, Adress* right) {
    print_str(left->city);
    print_str(right->city);
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


Post* find_post(Post* posts, const My_string* id, int size) {
    if (!id->size) {
        return NULL;
    }
    Adress* check = NULL;
    status_code st_adress = create_adress(&check, id);
    if (st_adress == code_error_alloc) {
        return NULL;
    }
    print_str(check->index);
    int _size = size;
    Post* current;
    printf("%d--\n", _size);
    for (int i = 0; i < _size; i++) {
        current = &posts[i];
        printf("%d--\n", i);
        //print_str(current->cur_id->index);
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
    sscanf(tmp->data, "%d:%d:%d %d:%d:%d", day, month, year, hour, minute, seconds);
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
    int diff_id = my_strcmp(((Mail*)a)->ad.index, ((Mail*)b)->ad.index);
    if (diff_id > 0) {
        return 1;
    } else if (diff_id < 0) {
        return -1;
    }
    return my_strcmp(((Mail*)a)->id, ((Mail*)b)->id);
}

status_code find_expired_mails(Post* post, Mail** exp_mails, int* size) {
    if (!post) {
        return code_invalid_parameter;
    }
    (*exp_mails) = malloc(sizeof(Mail*) * (*size));
    if (*exp_mails == NULL) {
        return code_error_alloc;
    }
    int tmp_size = post->size;
    Mail* current = &post->mails[0];
    My_string* null_string = String("");
    int index = 0;
    for (int i = 0; i < tmp_size; i++) {
        if (current->time_get || is_equal(null_string, current->time_get)) {
            (*exp_mails)[index] = *current;
            index++;
            if (index == *size - 1) {
                *size *= 2;
                status_realloc st_realloc = my_realloc(exp_mails, *size);
            }
        }
    }
    return code_success;
}

status_code find_non_expired_mails(Post* post, Mail** exp_mails, int* size) {
    if (!post) {
        return code_invalid_parameter;
    }
    (*exp_mails) = malloc(sizeof(Mail*) * (*size));
    if (*exp_mails == NULL) {
        return code_error_alloc;
    }
    int tmp_size = post->size;
    Mail* current = &post->mails[0];
    My_string* null_string = String("");
    int index = 0;
    for (int i = 0; i < tmp_size; i++) {
        if (!current->time_get || !is_equal(null_string, current->time_get)) {
            (*exp_mails)[index] = *current;
            index++;
            if (index == *size - 1) {
                *size *= 2;
                status_realloc st_realloc = my_realloc(exp_mails, *size);
            }
        }
    }
    return code_success;
}

int compare_mails_date_expired(const void* a, const void* b) {
    Mail* left = (Mail*)a;
    Mail* right = (Mail*)b;
    My_string* left_date_create = left->time_create;
    My_string* right_date_create = left->time_create;
    int l_day, l_month, l_year, l_hour, l_minute, l_sec;
    int r_day, r_month, r_year, r_hour, r_minute, r_sec;
    get_date(left_date_create, &l_day, &l_month, &l_year, &l_hour, &l_minute, &l_sec);
    get_date(right_date_create, &r_day, &r_month, &r_year, &r_hour, &r_minute, &r_sec);
    int diff_year = l_year - r_year;
    if (diff_year != 0) return diff_year;
    int diff_month = l_month - r_month;
    if (diff_month != 0) return diff_month;
    int diff_day = l_day - r_day;
    if (diff_day != 0) return diff_day;
    int diff_hour = l_hour - r_hour;
    if (diff_hour != 0) return diff_hour;
    int diff_min = l_minute - r_minute;
    if (diff_min != 0) return diff_min;
    int diff_sec = l_sec - r_sec;
    if (diff_sec != 0) return diff_sec;
    return 0;
}

int compare_mails_date_non_expired(const void* a, const void* b) {
    Mail* left = (Mail*)a;
    Mail* right = (Mail*)b;
    My_string* left_date_get = left->time_get;
    My_string* right_date_get = left->time_get;
    int l_day, l_month, l_year, l_hour, l_minute, l_sec;
    int r_day, r_month, r_year, r_hour, r_minute, r_sec;
    get_date(left_date_get, &l_day, &l_month, &l_year, &l_hour, &l_minute, &l_sec);
    get_date(left_date_get, &r_day, &r_month, &r_year, &r_hour, &r_minute, &r_sec);
    int diff_year = l_year - r_year;
    if (diff_year != 0) return diff_year;
    int diff_month = l_month - r_month;
    if (diff_month != 0) return diff_month;
    int diff_day = l_day - r_day;
    if (diff_day != 0) return diff_day;
    int diff_hour = l_hour - r_hour;
    if (diff_hour != 0) return diff_hour;
    int diff_min = l_minute - r_minute;
    if (diff_min != 0) return diff_min;
    int diff_sec = l_sec - r_sec;
    if (diff_sec != 0) return diff_sec;
    return 0;
}

void print_mails(Mail* mails, int size) {
    if (!mails || !size) {
        return;
    }
    printf("|===================================================================================|\n");
    printf("|       Id      |    Weight   |        Time create       |        Time get          |\n");
    printf("|===================================================================================|\n");
    Mail* current;
    for (int i = 0; i < size; i++) {
        current = &mails[i]; 
        printf("|%14s | %11lf | %24s | %24s |\n", current->id->data, current->weight, current->time_create->data, current->time_get->data);
    }
    printf("|===================================================================================|\n");
}

status_cmd command(char** arg_one, char** arg_two, My_string** info) {
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
        return cmd_sort;
    } else if (!strcmp(cmd, "Table")) {
        free(cmd);
        return cmd_table;
    } else if (!strcmp(cmd, "Create")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        return cmd_create_post;
    } else if (!strcmp(cmd, "Change")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        return cmd_change_post;
    } else if (!strcmp(cmd, "Add")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        return cmd_add_mail;
    } else if (!strcmp(cmd, "Remove")) {
        free(cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return cmd_error_alloc;
        }
        while(isspace(symbol)) {
            symbol = getchar();
        }
        while (symbol != '\n') {
            (*arg_one)[index++] = symbol;
            symbol = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        (*info) = String(*arg_one);
        return cmd_remove_mail;
    } else if (!strcmp(cmd, "Print")) {
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
        if (!strcmp("expired", *arg_two)) {
            free(*arg_one);
            free(*arg_two);
            return cmd_print_expired;
        }
        if (!strcmp("non-expired", *arg_two)) {
            free(*arg_one);
            free(*arg_two);
            return cmd_print_non_expired;
        }
    } else {
        free(cmd);
        return cmd_invalid_parameter;
    }
}

status_code free_post(Post* post) {
    if (free_adress(post->cur_id) != 1) {
        return code_invalid_parameter;
    }
    int _size = post->size;
    Mail* current;
    for (int i = 0; i < _size; i++) {
        current = &post->mails[i];
        string_clear(current->id);
        string_clear(current->time_create);
        string_clear(current->time_get);
        if (free_adress(&current->ad) != 1) {
            return code_invalid_parameter;
        }
    }
    post->size = 0;
    post->capacity = 1;
    return code_success;
}

status_code free_storage(Post** posts, int size) {
    if (!(*posts)) {
        return code_invalid_parameter;
    }
    for (int i = 0; i < size; i++) {
        free_post(posts[i]);
    }
    free(*posts);
    return code_success;
}