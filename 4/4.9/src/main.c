#include "../../4.9/include/utils/headers/user_functions.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Error number of args!\n");
        return -1;
    }
    Logger* logger = NULL;
    status_code st_act;
    create_log(&logger, "started main program\n", STARTED, NULL, NULL, 0, get_time_now());
    write_log(logger);
    Model* model = NULL;
    Request* reqs = NULL;
    Post* posts = NULL;
    int count_reqs, count_posts;
    int choose;
    print_choose();
    scanf("%d", &choose);
    st_act = get_all_info(logger, choose, argc, argv, &model, reqs, &posts, &count_posts, &count_reqs);
    if (st_act != code_success) {
        create_log(&logger, "error catched after get_all_info, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return -2;
    }
    my_time start = model->start_time;
    my_time end = model->end_time;
    int cmp_time = compare_time(&start, &end);
    int post_tmp_index = 0;
    int request_index = 0;
    for (int minute = 1; cmp_time >= 0; add_minute(&start)) {
        Request to_process = reqs[request_index];
        if (compare_time(&to_process.sending_time, &start) == 0) {
            
        }
    }
}