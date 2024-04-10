#include "headers/user_functions.h"


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
            return code_invalid_parameter;
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
            st_act = create_queue_bh(&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case binomial:
            st_act = create_binom_heap(&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case fibonacci:
            st_act = create_fib_heap(&(*post)->storage);
            if (st_act != code_success) {
                create_log(&logger, "didnt allocate on heap\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
                write_log(logger);
                free_post(*post, type_heap);
                return st_act;
            }
            break;
        case skew:
            (*post)->storage = NULL;
            break;
        case treap:
            (*post)->storage = NULL;
            break;
        case leftist:
            (*post)->storage = NULL;
            break;
        default:
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
    } 
}

Storage get_type_map(char* str) {
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
    (*posts_nodes) = (Post**)malloc(sizeof(Post*) * (*model)->count_post);
    if (!*posts_nodes) {
        create_log(&logger, "didnt allocate on post_nodes\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        free(line);
        fclose(file);
        return code_error_alloc;
    }
    while (check = sscanf(line, "%d ", &num)) {
        if (check == 1) {
            Post* tmp = NULL;
            st_act = create_post(logger, &tmp, (*model)->heap_type, num);
            if (st_act != code_success) {
                create_log(&logger, "ERORR check log status\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                free(line);
                fclose(file);
                return st_act;
            }
            (*posts_nodes)[*size_posts] = tmp;
            *size_posts++;
            
        } else {
            create_log(&logger, "invalid num\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
            write_log(logger);
            free(line);
            fclose(file);
            return code_invalid_parameter;
        }
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
    sscanf(line, "%lf", &(*model)->coeff_overload);
    free(line);
    line = NULL;
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

    // Prompt and get post numbers from user
    printf("Enter post numbers: ");
    for (int i = 0; i < (*model)->count_post; ++i) {
        if (!fgets(line, sizeof(line), stdin)) {
            create_log(&logger, "didnt read post numbers\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
            write_log(logger);
            free(*model);
            free(*posts_nodes);
            return code_invalid_parameter;
        }
        int num;
        sscanf(line, "%d", &num);
        Post* tmp = NULL;
        st_act = create_post(&tmp, (*model)->heap_type, num);
        if (st_act != code_success) {
            free(*model);
            free(*posts_nodes);
            create_log(&logger, "didnt create post, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            return st_act;
        }
        (*posts_nodes)[*size_posts] = tmp;
        (*size_posts)++;
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
    st_act = create_map((*model)->storage_type, *model);
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
            st_act = create_array(&st, model->count_post);
            if (st_act != code_success) {
                create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case bst:
            break;
        case trie:
            st_act = create_trie(&st);
            if (st_act != code_success) {
                create_log(&logger, "didnt create map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case hashset:
            st_act = create_table(&st, model->count_post * 2);
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

status_code read_request(Logger* logger, int count_files, char* filenames[], Request** reqs, int* total_reqs) {
    create_log(&logger, "read_request started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (count_files < 1) {
        create_log(&logger, "not enough args\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_oppening;
    }
    *total_reqs = 0;
    (*reqs) = (Request*)malloc(sizeof(Request));
    if (!*reqs) {
        create_log(&logger, "didnt allocate on requests\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_error_alloc; 
    }
    status_code st_act;
    for (int i = 0; i < count_files; i++) {
        FILE* tmp = fopen(filenames[i + 3], "r");
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
            st_act = create_request(&_req, line);
            if (st_act != code_success) {
                for (int j = 0; j < *total_reqs; j++) {
                    free_request(&(*reqs)[j]);
                }
                create_log(&logger, "Error in create_request", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                free(*reqs);
                return st_act;
            }
            (*reqs)[*total_reqs] = _req;
            *total_reqs++;
            free(line);
            line = NULL;
        }
        free(line);
        line = NULL;
        fclose(tmp);

    }
    create_log(&logger, "read_request started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;

}

status_code get_all_info(Logger* logger, int choose, int argc, char* argv[], Model* model, Request* reqs, Post** posts_nodes, int* size_posts, int* count_reqs) {
    if (argc < 1 || !argv) {
        create_log(&logger, "not enough args or empty argv\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    int max_prior;
    sscanf(argv[1], "%d", &max_prior);
    create_log(&logger, "get_all_info function started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    status_code st_act;
    if (choose == 1) {
        create_log(&logger, "params will be written by user\n", INFO, NULL, NULL, 0, get_time_now());
        write_log(logger);
        st_act = get_full_model_from_user(logger, &model, &posts_nodes, &size_posts);
    } else {
        create_log(&logger, "params will be written from file\n", INFO, NULL, NULL, 0, get_time_now());
        write_log(logger);
        st_act = get_full_model(logger, &model, argv[2], &posts_nodes, &size_posts);
    }
    if (st_act != code_success) {
        create_log(&logger, "ERROR detected, check status of log\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }

    int new_count = argc - 2;
    st_act = read_request(logger, new_count, argv, &reqs, &count_reqs);
    if (st_act != code_success) {
        create_log(&logger, "ERROR detected, check status of log\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    st_act = create_map(logger, model->storage_type, model);
    if (st_act != code_success) {
        create_log(&logger, "ERROR detected, check status of log\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    create_log(&logger, "get_all_info function has finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}

status_code insert_map(Logger* logger, Model* model, Request to_add, Post** posts, int count_posts, int* index_post) {
    create_log(&logger, "insert_map function has started\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    if (!model) {
        create_log(&logger, "model is empty\n", INVALID_PARAMETER, NULL, NULL, 0, get_time_now());
        write_log(logger);
        return code_invalid_parameter;
    }
    bool is_exist = false;
    Post* tmp = NULL;
    status_code st_act;
    switch(model->storage_type) {
        case array:
            tmp = search_array(model->map, to_add.index);
            break;
        case bst:
            tmp = search_bst(model->map, to_add.index);
            break;
        case trie:
            tmp = search_trie(model->map, to_add.index);
            break;
        case hashset:
            tmp = find_element(model->map, to_add.index);
            break;
    }
    if (!tmp) {
        tmp = posts[*index_post];
        *index_post++;
        switch(model->storage_type) {
            case array:
                st_act = insert_array(model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);

                    return st_act;
                }
                break;
            case bst:
                st_act = insert_tree(&model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                break;
            case trie:
                st_act = insert_trie(model->map, to_add.index, tmp);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                break;
            case hashset:
                st_act = insert_table(&model->map, to_add.index, tmp, free_table);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying inserting in map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                break;
        }
    } 
    switch(model->heap_type) {
        case binary:
            st_act = insert_bh(tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case binomial:
            st_act = insert_binomial(tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case fibonacci:
            st_act = insert_fib(&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case skew:
            st_act = insert_skew(&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
        case treap:
            insert_treap(&tmp->storage, to_add);
            break;
        case leftist:
            st_act = insert_lh(&tmp->storage, to_add);
            if (st_act != code_success) {
                create_log(&logger, "error catched while trying inserting in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                write_log(logger);
                return st_act;
            }
            break;
    }
    int size_storage;
    switch(model->heap_type) {
        case binary:
            size_storage = binary_heap_size(tmp->storage);
            break;
        case binomial:
            size_storage = size_binom_heap(tmp->storage);
            break;
        case fibonacci:
            size_storage = size_fib_heap(tmp->storage);
            break;
        case skew:
            size_storage = skew_heap_size(tmp->storage);
            break;
        case treap:
            size_storage = treap_size(tmp->storage);
            break;
        case leftist:
            size_storage = leftist_heap_size(tmp->storage);
            break;
    }
    double coeff_over = size_storage / tmp->size_operators;
    if (fabs(coeff_over - model->coeff_overload) > 1e-8) {
        tmp->is_overload = true;
        Post* exchange = find_less_load(posts, count_posts);
        if (!exchange) return code_success;
        switch(model->heap_type) {
            case binary:
                st_act = merge_destruction_bh(exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                st_act = create_queue_bh(&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case binomial:
                st_act = merge_binom(&exchange->storage, exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                st_act = create_binom_heap(&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case fibonacci:
                exchange->storage = fib_merge_destr(exchange->storage, tmp->storage);
                st_act = create_fib_heap(&tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to create heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case skew:
                exchange->storage = skew_merge_destroy(exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case treap:
                exchange->storage = merge_treap(exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
            case leftist:
                exchange->storage = merge_lh(exchange->storage, tmp->storage);
                if (st_act != code_success) {
                    create_log(&logger, "error catched while trying to merge in heap, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
                    write_log(logger);
                    return st_act;
                }
                tmp->is_overload = false;
                break;
        }
    }
    create_log(&logger, "insert_map function has finished\n", INFO, NULL, NULL, 0, get_time_now());
    write_log(logger);
    return code_success;
}


Post* find_less_load(Post** posts, int size) {
    if (!posts) return NULL;
    int minimum = posts[0]->left_requests;
    Post* tmp = posts[0];
    for (int i = 1; i < size; i++) {
        if (posts[i]->left_requests < minimum && !posts[i]->is_overload) {
            minimum = posts[i]->left_requests;
            tmp = posts[i];
        }
    }
    return tmp;
}

void print_choose() {
    printf("Enter 1 to upload params of model by your self\n");
    printf("Enter 2 to upload params of model from file\n");
}

