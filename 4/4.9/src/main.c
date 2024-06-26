#include "../../4.9/include/utils/headers/user_functions.h"


void print_reqs(Request* reqs, int size) {
    for (int i = 0; i < size; i++) {
        Request tmp = reqs[i];
        printf("TIME: %02d-%02d-%02dT%02d:%02d:%02d\n", tmp.sending_time.year,tmp.sending_time.month, tmp.sending_time.day, tmp.sending_time.hour, tmp.sending_time.minute, tmp.sending_time.second); 
        printf("PRIOR: %d\n", tmp.priority);
        printf("id: %s\n", tmp.id);
        printf("index: %s\n", tmp.index);
        printf("text: %s\n", tmp.text);
        printf("\n");
    }
}

void print_posts(Post** posts, int size) {
    for (int i = 0; i < size; i++) {
        Post* tmp = posts[i];
        printf("free requests: %d\n", tmp->free_requests);
        printf("size operators: %d\n", tmp->size_operators);
        printf("is overload: %d\n", tmp->is_overload);
        for (int j = 0; j < tmp->size_operators; j++) {
            Operator _tmp = tmp->ops[j];
            printf("Operator name: %s\n", _tmp.name);
            printf("time process: %d\n", _tmp.time_process);
            print_time(_tmp.start);
            if (_tmp.current_request != NULL) {
                printf("text: %s\n", _tmp.current_request->text);
                printf("index: %s\n", _tmp.current_request->index);
                printf("zayavka %s\n", _tmp.current_request->id);
            }
            printf("\n");
        }
        printf("\n\n");
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    printf("%d--\n", argc);
    Logger* logger = NULL;
    status_code st_act;
    create_log(&logger, "started main program\n", STARTED, NULL, NULL, 0, get_time_now());
    write_log(logger);
    Model* model = NULL;
    Request* reqs = NULL;
    Post** posts = NULL;
    int count_reqs, count_posts;
    int choose;
    print_choose();
    scanf("%d", &choose);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    st_act = get_all_info(logger, choose, argc, argv, &model, &reqs, &posts, &count_posts, &count_reqs);
    printf("sstut\n");
    if (st_act != code_success) {
        printf("tut\n");
        create_log(&logger, "error catched after get_all_info, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return -2;
    }
    printf("SIZE REQS %d\n", count_reqs);
    //bubbleSort(reqs, count_reqs);
    my_time start = model->start_time;
    my_time end = model->end_time;
    int cmp_time = compare_time(&start, &end);
    printf("->>>%d\n", cmp_time);
    int post_tmp_index = 0;
    int request_index = 0;
    my_time _tmp = start;
    add_minute(&_tmp);
    printf("---------------%d\n", count_reqs);
    //print_info(posts, count_reqs, model);
    printf("---------------%d\n", count_reqs);
    //printf("->>>%d\n", compare_time(&start, &_tmp));
    printf("---------------%d\n", count_reqs);
    print_reqs(reqs, count_reqs);
    for (my_time start = model->start_time; compare_time(&start, &end) <= 0; add_minute(&start)) {
        Request to_process = reqs[request_index];
        //printf("%d\n", request_index);
        //print_time(start);
        if (compare_time(&to_process.sending_time, &start) == 0) {
            printf("ne zashel\n");
            printf("Current request: %s\n", to_process.id);
            printf("zdes2222\n");
            request_index++;
            st_act = insert_map(logger, model, to_process, posts, count_posts, &post_tmp_index, start);
            if (st_act != code_success) {
                create_log(&logger, "error catched after insert_map, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, start);
                write_log(logger);
                return -3;
            }
            //print_posts(posts, model->count_post);
            
        }
        process_requests(logger, model, posts, start);
    }
}