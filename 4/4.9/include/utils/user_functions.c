#include "headers/user_functions.h"

char* int_to_string(int number) {
    int length = snprintf(NULL, 0, "%d", number);
    char* str = (char*)malloc((length + 1) * sizeof(char));
    if (str == NULL) {
        return str;
    }
    snprintf(str, length + 1, "%d", number);
    return str;
}

int get_random_process_time(Model* model) {
    int st = rand() % model->min_process_time;
    int fl = rand() % model->max_process_time + st;
    return fl - st;
}

bool check_operator_free(Operator op) {
    return op.current_request == NULL;
}

char* get_id_post(Model* model, Post* post) {
    switch(model->storage_type) {
        case bst:
            return find_post_tree(model->map, post);
        case array:
            return find_post_array(model->map, post);
        case hashset:
            return find_post_hashset(model->map, post);
        case trie:
            return find_post_trie(model->map, post);
    }
    return NULL;
}


char* concatenate_strings(char* str1, char* str2, char* str3, char* str4) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t len3 = strlen(str3);
    size_t len4 = strlen(str4);
    size_t total_len = len1 + len2 + len3 + len4;

    char* result = (char*)malloc((total_len + 1) * sizeof(char));

    if (result == NULL) {
        return NULL;
    }

    memcpy(result, str1, len1);
    memcpy(result + len1, str2, len2);
    memcpy(result + len1 + len2, str3, len3);
    memcpy(result + len1 + len2 + len3, str4, len4);

    result[total_len] = '\0';

    return result;
}

void free_post(Post* post, Heap type_heap) {
    if (!post) return;
    for (int i = 0; i < post->size_operators; i++) {
        free_request(post->ops->current_request);
    }
    free(post->ops);
    switch(type_heap) {
        case binary:
            free_binary(post->storage);
            break;
        case binomial:
            free_binom_heap(post->storage);
            break;
        case fibonacci:
            free_fib_heap(post->storage);
            break;
        case skew:
            free_skew(post->storage);
            break;
        case treap:
            free_treap(post->storage);
            break;
        case leftist:
            free_leftist(post->storage);
            break;
        default:
            return;
    }
}

status_code create_post(Logger* logger, Post** post, Heap type_heap, int _size_op) {
    if (_size_op < 0)  return code_invalid_parameter;
    *post = (Post*)malloc(sizeof(Post));
    create_log(&logger, "creating post\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (!*post) {
        create_log(&logger, "didnt allocate on post\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_alloc;
    }
    (*post)->free_requests = _size_op;
    (*post)->is_overload = false;
    (*post)->size_operators = _size_op;
    status_code st_act;
    switch(type_heap) {
        case binary:
            printf("    _bin\n");
            st_act = create_queue_bh((Binary_heap**)&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case binomial:
            printf("    _binomial\n");
            st_act = create_binom_heap((Binomial_heap**)&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case fibonacci:
            printf("    _fib\n");
            st_act = create_fib_heap((Fibbonacci_heap**)&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case skew:
            printf("    _skew\n");
            (*post)->storage = NULL;
            break;
        case treap:
            printf("    _treap\n");
            (*post)->storage = NULL;
            break;
        case leftist:
            printf("    _leftist\n");
            (*post)->storage = NULL;
            break;
        default:
            printf("    _hz\n");
            create_log(&logger, "invalid parameter detected\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
            write_log(logger);
            return code_invalid_parameter;
    }
    (*post)->ops = (Operator*)malloc(sizeof(Operator) * _size_op);
    if (!(*post)->ops) {
        create_log(&logger, "didnt allocate on operators\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        if (st_act != code_success) {
            free_post(*post, type_heap);
            return st_act;
        }
    }
    for (int i = 0; i < _size_op; i++) {
        (*post)->ops[i].current_request = NULL;
        (*post)->ops[i].time_process = 0;
        get_unique_id((*post)->ops[i].name);

    }
    create_log(&logger, "creating post finished\n\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;

}

Heap get_type_heap(char* str) {
    // Удалить символ новой строки, если он есть
    str[strcspn(str, "\n")] = '\0';

    printf("[%s]\n", str);
    if (!strcmp(str, "BinaryHeap")) {
        return binary;
    } else if (!strcmp(str, "BinomialHeap")) {
        return binomial;
    } else if (!strcmp(str, "FibonacciHeap")) {
        return fibonacci;
    } else if (!strcmp(str, "SkewHeap")) {
        return skew;
    } else if (!strcmp(str, "LeftistHeap")) {
        return leftist;
    } else if (!strcmp(str, "Treap")) {
        return treap;
    } else {
        printf("hzshka\n");
    }
}


Storage get_type_map(char* str) {
    // Удалить символ новой строки, если он есть
    str[strcspn(str, "\n")] = '\0';

    if (!strcmp(str, "HashSet")) {
        return hashset;
    } else if (!strcmp(str, "DynamicArray")) {
        return array;
    } else if (!strcmp(str, "BinarySearchTree")) {
        return bst;
    } else if (!strcmp(str, "Trie")) {
        return trie;
    }
}


status_code get_full_model(Logger* logger, Model** model, const char* filename, Post*** posts_nodes, int* size_posts) {
    create_log(&logger, "get_full_model started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (!filename) {
        create_log(&logger, "Invalid filename, it`s empty\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    FILE* file = fopen(filename, "r");
    if (!file) {
        create_log(&logger, "cant open file\n", ERROR_OPPENING, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    status_code st_act;
    char* line = NULL;
    int read;
    (*model) = (Model*)malloc(sizeof(Model));
    if (!*model) {
        create_log(&logger, "didnt allocate on model\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        fclose(file);
        return code_error_alloc;
    }
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    printf("heap  <%s\n", line);
    (*model)->heap_type = get_type_heap(line);
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    printf("map  <%s\n", line);
    (*model)->storage_type = get_type_map(line);
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    st_act = get_iso_time(line, &(*model)->start_time);
    if (st_act != code_success) {
        create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    printf("TIME: %s\n", line);
    st_act = get_iso_time(line, &(*model)->end_time);
    if (st_act != code_success) {
        create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    sscanf(line, "%d", &(*model)->min_process_time);
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    sscanf(line, "%d", &(*model)->max_process_time);
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    sscanf(line, "%d", &(*model)->count_post);
    free(line);
    line = NULL;
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    *size_posts = 0;
    int num;
    int check;
    (*posts_nodes) = (Post**)malloc(sizeof(Post*) * (*model)->count_post + 1);
    if (!*posts_nodes) {
        create_log(&logger, "didnt allocate on post_nodes\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    printf("DOSHEL\n");
    char* token = strtok(line, " "); // Получаем первый токен
    *size_posts = (*model)->count_post;
    int _ind = 0;
    while (token != NULL) {
        if (sscanf(token, "%d", &num) == 1) {
            Post* tmp = NULL;
            printf("NUMBER = %d\n", num);
            st_act = create_post(logger, &tmp, (*model)->heap_type, num);
            if (st_act != code_success) {
                printf("wfd\n");
                create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                free(line);
                fclose(file);
                return st_act;
            }
            (*posts_nodes)[_ind] = tmp;
            _ind++;
            
        } else {
            create_log(&logger, "invalid num\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
            write_log(logger);
            free(line);
            fclose(file);
            return code_invalid_parameter;
        }
        token = strtok(NULL, " "); // Получаем следующий токен
    }
    free(line);
    line = NULL;
    printf("PEREDOSHEL\n");
    read = getline(&line, &(size_t){0}, file);
    if (read == -1) {
        create_log(&logger, "didnt allocate on line\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    sscanf(line, "%lf", &(*model)->coeff_overload);
    free(line);
    line = NULL;
    printf("coeff = %f\n", (*model)->coeff_overload);
    st_act = create_map(logger, (*model)->storage_type, *model);
    if (st_act != code_success) {
        create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
    }
    free(line);
    line = NULL;
    create_log(&logger, "get_full_model finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}

status_code get_full_model_from_user(Logger* logger, Model** model, Post*** posts_nodes, int* size_posts) {
    status_code st_act;
    create_log(&logger, "get_full_model_from_user started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    
    char line[256]; 
    (*model) = (Model*)malloc(sizeof(Model));
    if (!*model) {
        create_log(&logger, "didnt allocate on model\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_alloc;
    }
    printf("Enter heap type: ");
    if (!fgets(line, sizeof(line), stdin)) {
        create_log(&logger, "didnt read heap type\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return code_invalid_parameter;
    }
    (*model)->heap_type = get_type_heap(line);
    printf("Enter storage type: ");
    if (!fgets(line, sizeof(line), stdin)) {
        create_log(&logger, "didnt read storage type\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return code_invalid_parameter;
    }
    (*model)->storage_type = get_type_map(line);

    printf("Enter start time in ISO format: ");
    if (!fgets(line, sizeof(line), stdin)) {
        create_log(&logger, "didnt read time\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return code_invalid_parameter;
    }
    st_act = get_iso_time(line, &(*model)->start_time);
    if (st_act != code_success) {
        create_log(&logger, "didnt read time\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return st_act;
    } 
    printf("Enter end time in ISO format: ");
    if (!fgets(line, sizeof(line), stdin)) {
        create_log(&logger, "didnt read time\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return code_invalid_parameter;
    }
    st_act = get_iso_time(line, &(*model)->end_time);
    if (st_act != code_success) {
        create_log(&logger, "didnt read time\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return st_act;
    }

    printf("Enter min process time: ");
    if (!fgets(line, sizeof(line), stdin)) {
        free(*model);
        create_log(&logger, "didnt read min process time\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    sscanf(line, "%d", &(*model)->min_process_time);

    // Prompt and get max process time from user
    printf("Enter max process time: ");
    if (!fgets(line, sizeof(line), stdin)) {
        free(*model);
        create_log(&logger, "didnt read max process time\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    sscanf(line, "%d", &(*model)->max_process_time);

    // Prompt and get count of posts from user
    printf("Enter count of posts: ");
    if (!fgets(line, sizeof(line), stdin)) {
        free(*model);
        create_log(&logger, "didnt read count of posts\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    sscanf(line, "%d", &(*model)->count_post);

    // Allocate memory for posts_nodes
    (*posts_nodes) = (Post**)malloc(sizeof(Post*) * (*model)->count_post);
    if (!*posts_nodes) {
        create_log(&logger, "didnt allocate on post_nodes\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        return code_error_alloc;
    }

    if (!fgets(line, sizeof(line), stdin)) {
        free(*model);
        create_log(&logger, "didnt read count of posts\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    // Prompt and get post numbers from user
    char* token = strtok(line, " "); // Получаем первый токен
    *size_posts = (*model)->count_post;
    int _ind = 0;
    int num;
    while (token != NULL) {
        if (sscanf(token, "%d", &num) == 1) {
            Post* tmp = NULL;
            printf("NUMBER = %d\n", num);
            st_act = create_post(logger, &tmp, (*model)->heap_type, num);
            if (st_act != code_success) {
                printf("wfd\n");
                create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            (*posts_nodes)[_ind] = tmp;
            _ind++;
            
        } else {
            create_log(&logger, "invalid num\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
            write_log(logger);
            return code_invalid_parameter;
        }
        token = strtok(NULL, " "); // Получаем следующий токен
    }

    printf("Enter coefficient of overload: ");
    if (!fgets(line, sizeof(line), stdin)) {
        free(*model);
        free(*posts_nodes);
        create_log(&logger, "didnt read coefficient of overload\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    sscanf(line, "%lf", &(*model)->coeff_overload);

    // Create map
    st_act = create_map(logger, (*model)->storage_type, *model);
    if (st_act != code_success) {
        create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(*model);
        free(*posts_nodes);
        return st_act;
    }
    create_log(&logger, "get_full_model_from_user finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}

status_code create_map(Logger* logger, Storage type_storage, Model* model) {
    create_log(&logger, "create_map started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (!model) {
        create_log(&logger, "model is empty\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    void* st = NULL;
    status_code st_act;
    model->storage_type = type_storage;
    switch (type_storage) {
        case array:
            printf("Array zdes\n");
            st_act = create_array((Array**)&st, model->count_post);
            if (st_act != code_success) {
                create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case bst:
            break;
        case trie:
            st_act = create_trie((Trie**)&st);
            if (st_act != code_success) {
                create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case hashset:
            st_act = create_table((Hash_table**)&st, model->count_post * 2);
            if (st_act != code_success) {
                create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
    }
    model->map = st;
    create_log(&logger, "create_map finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}

status_code read_request(Logger* logger, int count_files, char* filenames[], Request** reqs, int* total_reqs, int choose) {
    create_log(&logger, "read_request started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (count_files < 1) {
        create_log(&logger, "not enough args\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_oppening;
    }
    *total_reqs = 0;
    int _capac = 64;
    (*reqs) = (Request*)malloc(sizeof(Request) * _capac);
    //printf("tuttt\n");
    if (!*reqs) {
        create_log(&logger, "didnt allocate on requests\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_alloc; 
    }
    //printf("tuttt2\n");
    status_code st_act;
    //printf("tutt3\n");
    int _ind = 0;
    for (int i = 0; i < count_files - 1; i++) {
        printf("%d-----\n", count_files - 1);
        //printf("tuttt5\n");
        FILE* tmp;
        if (choose == 1) {
            tmp = fopen(filenames[i + 2], "r");
        } else {
            tmp = fopen(filenames[i + 3], "r");
        }
        //printf("    %s tuttt6\n", filenames[i + 3]);
        if (!tmp) {
            char* log_m = strdup(" cant open file\n");
            char* log_message = strcat(filenames[i + 3], log_m);
            create_log(&logger, log_message, ERROR_OPPENING, NULL, NULL, 0, get_time_now());
            write_log(logger);
            for (int j = 0; j < *total_reqs; j++) {
                free_request(&(*reqs)[j]);
            }
            free(*reqs);
            return code_error_oppening;
        }
        char* line = NULL;
        int read;
        while (read = getline(&line, &(size_t){0}, tmp) != -1) {
            if (read == -1) {
                create_log(&logger, "didnt allocate on line", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free(line);
                fclose(tmp);
                for (int j = 0; j < *total_reqs; j++) {
                    free_request(&(*reqs)[j]);
                }
                free(*reqs);
                return code_error_alloc;
            }
            Request _req;
            //printf("tuttt7\n");
            printf("    LINE: %s\n", line);
            st_act = create_request(&_req, line);
            printf("\t<CREATED>\n");
            if (st_act != code_success) {
                printf("ZAEBIS BYAHA\n");
                for (int j = 0; j < *total_reqs; j++) {
                    free_request(&(*reqs)[j]);
                }
                create_log(&logger, "Error in create_request", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                free(*reqs);
                return st_act;
            }
            printf("\t<went>\n");
            //printf("       %s %s %s\n", _req.id, _req.index, _req.text);
            printf("<<%d>>\n", *total_reqs);
            (*reqs)[_ind] = _req;
            _ind++;
            printf("\t<added>\n");
            if (_ind == _capac - 1) {
                printf("here\n");
                _capac *= 2;
                Request* _tmp = (Request*)malloc(sizeof(Request) * _capac);
                if (_tmp) {
                    *reqs = _tmp;
                } else {
                    create_log(&logger, "didnt allocate on line", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    free(line);
                    fclose(tmp);
                    for (int j = 0; j < *total_reqs; j++) {
                        free_request(&(*reqs)[j]);
                    }
                    free(*reqs);
                    return code_error_alloc;
                }
            }
            if (line) free(line);
            if (line) line = NULL;
        }
        *total_reqs = _ind;
        printf("HERE\n");
        if (line) free(line);
        if (line) line = NULL;
        fclose(tmp);

    }

    create_log(&logger, "read_request finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;

}

void print_info(Post** posts, int size, Model* model) {
    printf("%d %f %d %d", model->count_post, model->coeff_overload, model->min_process_time, model->max_process_time);
    for (int i = 0; i < size; i++) {
        Post* tmp = posts[i];
        printf("    SIZE = %d\n", tmp->size_operators);
        printf("%d %d\n", tmp->free_requests, tmp->is_overload);
        for (int j = 0; j < tmp->size_operators; j++) {
            printf("id = %s\n", tmp->ops[j].name);
        }
    }
}

status_code get_all_info(Logger* logger, int choose, int argc, char* argv[], Model** model, Request** reqs, Post*** posts_nodes, int* size_posts, int* count_reqs) {
    if (argc < 1 || !argv) {
        create_log(&logger, "not enough args or empty argv\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        //printf("zdes\n");
        return code_invalid_parameter;
    }
    int max_prior;
    sscanf(argv[1], "%d", &max_prior);
    printf("    prior = %d\n", max_prior);
    create_log(&logger, "get_all_info function started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    status_code st_act;
    if (choose == 1) {
        create_log(&logger, "params will be written by user\n", INFO, NULL, NULL, 0, get_time_now());
        write_log(logger);
        st_act = get_full_model_from_user(logger, model, posts_nodes, size_posts);
    } else {
        create_log(&logger, "params will be written from file\n", INFO, NULL, NULL, 0, get_time_now());
        write_log(logger);
        st_act = get_full_model(logger, model, argv[2], posts_nodes, size_posts);
    }
    if (st_act != code_success) {
        create_log(&logger, "ERROR detected, check status of log\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }

    int new_count = argc - 2;
    print_info(*posts_nodes, (*model)->count_post, *model);
    st_act = read_request(logger, new_count, argv, reqs, count_reqs, choose);
    if (st_act != code_success) {
        create_log(&logger, "ERROR detected, check status of log\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    printf("doshel\n");
    create_log(&logger, "get_all_info function has finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}

status_code insert_map(Logger* logger, Model* model, Request to_add, Post** posts, int count_posts, int* index_post, my_time _time) {
    create_log(&logger, "insert_map function has started\n", INFO, NULL, NULL, 0, _time);
    write_log(logger);
    if (!model) {
        create_log(&logger, "model is empty\n", INVALID_PARAMETER, NULL, NULL, 0, _time);
        write_log(logger);
        return code_invalid_parameter;
    }
    bool is_exist = false;
    Post* tmp = NULL;
    status_code st_act;
    switch(model->storage_type) {
        case array:
            tmp = search_array((Array*)model->map, to_add.index);
            break;
        case bst:
            tmp = search_bst((Tree*)model->map, to_add.index);
            break;
        case trie:
            tmp = search_trie((Trie*)model->map, to_add.index);
            break;
        case hashset:
            tmp = find_element((Hash_table*)model->map, to_add.index);
            break;
    }
    if (!tmp) {
        tmp = posts[*index_post];
        *index_post++;
        switch(model->storage_type) {
            case array:
                st_act = insert_array(logger, (Array*)model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                    write_log(logger);

                    return st_act;
                }
                break;
            case bst:
                st_act = insert_tree(logger, (Tree**)&model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                    write_log(logger);
                    return st_act;
                }
                break;
            case trie:
                st_act = insert_trie(logger, (Trie*)model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                    write_log(logger);
                    return st_act;
                }
                break;
            case hashset:
                st_act = insert_table(model, logger, (Hash_table**)&model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                    write_log(logger);
                    return st_act;
                }
                break;
        }
    } 
    switch(model->heap_type) {
        case binary:
            st_act = insert_bh(logger, (Binary_heap*)tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                write_log(logger);
                return st_act;
            }
            break;
        case binomial:
            st_act = insert_binomial(logger, (Binomial_heap*)tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                write_log(logger);
                return st_act;
            }
            break;
        case fibonacci:
            st_act = insert_fib(logger, (Fibbonacci_heap**)&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                write_log(logger);
                return st_act;
            }
            break;
        case skew:
            st_act = insert_skew(logger, (Skew_heap**)&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                write_log(logger);
                return st_act;
            }
            break;
        case treap:
            insert_treap(logger, (Treap**)&tmp->storage, to_add);
            break;
        case leftist:
            st_act = insert_lh(logger, (Leftist_heap**)&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                write_log(logger);
                return st_act;
            }
            break;
    }
    int size_storage;
    switch(model->heap_type) {
        case binary:
            size_storage = binary_heap_size((Binary_heap*)tmp->storage);
            break;
        case binomial:
            size_storage = size_binom_heap((Binomial_heap*)tmp->storage);
            break;
        case fibonacci:
            size_storage = size_fib_heap((Fibbonacci_heap*)tmp->storage);
            break;
        case skew:
            size_storage = skew_heap_size((Skew_heap*)tmp->storage);
            break;
        case treap:
            size_storage = treap_size((Treap*)tmp->storage);
            break;
        case leftist:
            size_storage = leftist_heap_size((Leftist_heap*)tmp->storage);
            break;
    }
    double coeff_over = size_storage / tmp->size_operators;
    tmp->free_requests = tmp->size_operators - size_storage;
    char* information = concatenate_strings("new request id and post index: ", to_add.id, ", ", to_add.index);
    create_log(&logger, information, NEW_REQUEST, NULL, NULL, 0, _time);
    write_log(logger);
    if (fabs(coeff_over - model->coeff_overload) < 1e-8) {
        tmp->is_overload = true;
        Post* exchange = find_less_load(posts, count_posts);
        if (!exchange) return code_success;
        tmp->free_requests = tmp->size_operators;
        char* id_post = get_id_post(model, exchange);
        char* inform = concatenate_strings("post overloaded, ", to_add.id, "delivered to ", id_post);
        create_log(&logger, inform, DEPARTMENT_OVERLOADED, NULL, id_post, 0, _time);
        write_log(logger);
        switch(model->heap_type) {
            case binary:
                st_act = merge_destruction_bh(logger, (Binary_heap*)exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                st_act = create_queue_bh((Binary_heap**)&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case binomial:
                st_act = merge_binom(logger, (Binomial_heap**)&exchange->storage, (Binomial_heap*)exchange->storage, (Binomial_heap*)tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                st_act = create_binom_heap((Binomial_heap**)&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case fibonacci:
                exchange->storage = fib_merge_destr(logger, (Fibbonacci_heap*)exchange->storage, (Fibbonacci_heap*)tmp->storage);
                st_act = create_fib_heap((Fibbonacci_heap**)&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case skew:
                exchange->storage = skew_merge_destroy(logger, (Skew_heap*)exchange->storage, (Skew_heap*)tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case treap:
                exchange->storage = merge_treap(logger, (Treap*)exchange->storage, (Treap*)tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case leftist:
                exchange->storage = merge_lh((Leftist_heap*)exchange->storage, (Leftist_heap*)tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
        }
        tmp->free_requests = tmp->size_operators;
        for (int oper = 0; oper < tmp->size_operators; oper++) {
            tmp->ops[oper].current_request = NULL;
        }
        if (exchange->free_requests != 0) {
            for (int k = 0; k < exchange->size_operators; k++) {
                if (check_operator_free(exchange->ops[k])) {
                    exchange->ops[k].current_request = &to_add;
                    exchange->ops[k].start = to_add.sending_time;
                    exchange->ops[k].time_process = get_random_process_time(model);
                    information = concatenate_strings("request handling started ", to_add.id, ", ", tmp->ops[k].name);
                    create_log(&logger, information, REQUEST_HANDLING_STARTED, exchange->ops[k].name, to_add.index, 0, _time);
                    write_log(logger);
                    exchange->free_requests--;
                    free(information);
                    information = NULL;
                }
            }
        }
    } else {
        if (tmp->free_requests != 0) {
            for (int k = 0; k < tmp->size_operators; k++) {
                if (check_operator_free(tmp->ops[k])) {
                    tmp->ops[k].current_request = &to_add;
                    tmp->ops[k].start = to_add.sending_time;
                    tmp->ops[k].time_process = get_random_process_time(model);
                    information = concatenate_strings("request handling started ", to_add.id, ", ", tmp->ops[k].name);
                    create_log(&logger, information, REQUEST_HANDLING_STARTED, tmp->ops[k].name, to_add.index, 0, _time);
                    write_log(logger);
                    tmp->free_requests--;
                    free(information);
                    information = NULL;
                }
            }
        }
    }
    if (information) free(information);
    create_log(&logger, "insert_map function has finished\n", INFO, NULL, NULL, 0, _time);
    write_log(logger);
    return code_success;
}

Post* find_less_load(Post** posts, int size) {
    if (!posts) return NULL;
    int minimum = posts[0]->free_requests;
    Post* tmp = posts[0];
    for (int i = 1; i < size; i++) {
        if (posts[i]->free_requests < minimum && !posts[i]->is_overload) {
            minimum = posts[i]->free_requests;
            tmp = posts[i];
        }
    }
    return tmp;
}

void print_choose() {
    printf("Enter 1 to upload params of model by your self\n");
    printf("Enter 2 to upload params of model from file\n");
}

void process_requests(Logger* logger, Model* model, Post** posts, my_time _time) {
    for (int i = 0; i < model->count_post; i++) {
        Post* tmp = posts[i];
        Request _check;
        status_code st_act;
        switch(model->heap_type) {
            case binary:
                _check = ((Binary_heap*)tmp->storage)->data[0];
                break;
            case binomial:
                _check = ((Binomial_heap*)tmp->storage)->root->data;
                break;
            case fibonacci:
                _check = ((Fibbonacci_heap*)tmp->storage)->root->req;
                break;
            case skew:
                _check = ((Skew_heap*)tmp->storage)->data;
                break;
            case treap:
                _check = ((Treap*)tmp->storage)->data;
                break;
            case leftist:
                _check = ((Leftist_heap*)tmp->storage)->key;
                break;
        }
        char* op_replacer = NULL;
        for (int k = 0; k < tmp->size_operators; k++) {
            if (strcmp(_check.id, tmp->ops[k].current_request->id)) {
                continue;
            }
            my_time check = _time;
            add_minutes(&check, tmp->ops[k].time_process);
            
            if (compare_time(&_time, &_check.sending_time) >= 0) {
                char* str_op = strdup(tmp->ops[k].name);
                str_op = strcat(str_op, " ");
                int diff_t = time_difference_minutes(_time, _check.sending_time);
                char* diff_t_str = int_to_string(diff_t);
                str_op = strcat(str_op, diff_t_str);
                char* information = concatenate_strings("request handling finished: ", _check.id, ", operator and time ", str_op);
                create_log(&logger, information, REQUEST_HANDLING_FINISHED, NULL, NULL, 0, _time);
                write_log(logger);
                free(information);
                information = NULL;
                Request* to_del = NULL;
                op_replacer = str_op;
                switch(model->heap_type) {
                    case binary:
                        to_del = pop_bh((Binary_heap*)tmp->storage);
                        break;
                    case binomial:
                        st_act = pop_binom_heap(logger, (Binomial_heap*)tmp->storage, to_del);
                        if (st_act != code_success) {
                            create_log(&logger, "error after popping", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                            write_log(logger);
                            return;
                        }
                        break;
                    case fibonacci:
                        extract_max_fib((Fibbonacci_heap*)tmp->storage, to_del);
                        break;
                    case skew:
                        st_act = get_max_skew(logger, (Skew_heap**)&tmp->storage, to_del);
                        if (st_act != code_success) {
                            create_log(&logger, "error after popping", get_sev_from_status(st_act), NULL, NULL, 0, _time);
                            write_log(logger);
                            return;
                        }
                        break;
                    case treap:
                        erase_treap(logger, (Treap**)&tmp->storage, to_del);
                        break;
                    case leftist:
                        to_del = pop_lh((Leftist_heap**)&tmp->storage);
                        break;
                }
                free(to_del);
                to_del = NULL;
            }
            break;
        }
        tmp->free_requests++;
        //сделать что-то с переназначением заявки на оператора, когда он прекратил обработку, надо дать ему другую
    }
}
