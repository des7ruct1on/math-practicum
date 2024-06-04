#include "../../4.9/include/utils/headers/user_functions.h"
#include "../../4.9/include/utils/headers/logger.h"
#include "../../4.9/include/utils/headers/struct.h"


void get_random_dates(my_time* start, my_time* end) {
    srand(time(NULL));
    if (start) {
        start->year = rand() % (2024 - 2000 + 1) + 2000; // Ограничимся периодом 2000-2024
        start->month = rand() % 12 + 1; 
        start->day = rand() % 31 + 1; 
        start->hour = rand() % 24; 
        start->minute = rand() % 60; 
        start->second = rand() % 60; 
    }
    if (end) {
        end->year = rand() % (2024 - start->year) + start->year; // Год окончания не позднее года начала
        end->month = rand() % 12 + 1; 
        end->day = rand() % 31 + 1; 
        end->hour = rand() % 24; 
        end->minute = rand() % 60; 
        end->second = rand() % 60; 
    }
}

void get_random_date_diap(my_time A, my_time B, my_time* res) {

    res->year = A.year;//rand() % (B.year - A.year) + A.year;
    res->month = rand() % 12 + 1;
    res->day = rand() % 30 + 1;
    res->hour = rand() % 24;
    res->minute = rand() % 60;
    res->second = rand() % 60;
}

void write_requests(Logger* logger, char* filename, my_time st, my_time end, int prior) {
    //printf("%s %d\n", filename, prior);
    FILE* file = fopen(filename, "w");
    my_time cur_time = get_time_now();
    //printf("here22\n");
    create_log(&logger, "random generation requests function started\n", STARTED, NULL, NULL, 0, cur_time);
    write_log(logger);
    //printf("here23\n");
    if (!file) {
        create_log(&logger, "didn`t open file in random.c\n", ERROR_OPPENING, NULL, NULL, 0, cur_time);
        write_log(logger);
        return;
    }
    int _prior = prior;
    //printf("here27\n");
    for (int i = 0; i < 5; i++) {
        //printf("here30%d\n", i);
        my_time req_time;
        //if (prior == 0) printf("kek12\n");
        get_random_date_diap(st, end, &req_time);
        //printf("here-030%d\n", i);
        //printf("here    30%d\n", i);
        if (prior == 0) printf("kek1\n");
        fprintf(file, "%d-%02d-%02dT%02d:%02d:%02d ", req_time.year, req_time.month, req_time.day, req_time.hour, req_time.minute, req_time.second);

        //printf("%d-%2d-%2dT%d:%d:%d\n", req_time.year, req_time.month, req_time.day, req_time.hour, req_time.minute, req_time.second);
        if (prior == 0) printf("kek\n");
        printf("%d -- %d\n", prior, _prior);
        int cur_prior = 1 + rand() % prior;
        //printf("%d---\n", cur_prior);
        fprintf(file, "%d ", cur_prior);
        //printf("here        30%d\n", i);
        char id[256];
        char text[256];
        get_unique_id(id);
        //printf("str30%d\n", i);
        get_unique_id(text);
        //printf("str 30%d\n", i);
        fprintf(file, "%s ", id);
        fprintf(file, "%s\n", text);
    }
    //printf("here50\n");
    fclose(file);


}

int main(int argc, char* argv[]) {
    //printf("here\n");
    srand(time(NULL));
    FILE* file = fopen("src/model.txt", "w");
    Logger* logger = NULL;
    my_time cur_time = get_time_now();
    //printf("here2\n");
    create_log(&logger, "random generation function started\n", STARTED, NULL, NULL, 0, cur_time);
    //printf("here3\n");
    write_log(logger);
    //printf("here4\n");
    if (!file) {
        create_log(&logger, "didn`t open file in random.c\n", ERROR_OPPENING, NULL, NULL, 0, cur_time);
        write_log(logger);
        return -1;
    }
    //printf("here5\n");
    int random_index;
    char* heaps[] = {"BinaryHeap", "BinomialHeap", "FibonacciHeap", "SkewHeap", "LeftistHeap", "Treap"};
    char* maps[] = {"HashSet", "DynamicArray", "BinarySearchTree", "Trie"};
    int num_heaps = 6;
    int num_maps = 4;
    srand(time(NULL));
    random_index = rand() % num_heaps;
    fprintf(file, "%s\n", heaps[random_index]);
    random_index = rand() % num_maps;
    fprintf(file, "%s\n", maps[random_index]);
    //printf("here6\n");
    my_time st, end;
    get_random_dates(&st, &end);
    fprintf(file, "%04d-%02d-%02dT%02d:%02d:%02d\n", st.year, st.month, st.day, st.hour, st.minute, st.second);
    fprintf(file, "%04d-%02d-%02dT%02d:%02d:%02d\n", end.year, end.month, end.day, end.hour, end.minute, end.second);
    //printf("here7\n");
    int min_time = rand() % 60 + 1;
    
    int max_time = min_time + (rand() % (120 - min_time + 1));
    //printf("here8\n");
    fprintf(file, "%d\n", min_time);
    fprintf(file, "%d\n", max_time);
    int count_posts = rand() %  100 + 1;
    //printf("here9\n");
    fprintf(file, "%d\n", count_posts);
    for (int i = 0; i < count_posts; i++) {
        int num_ops = rand() % 50 + 10;
        fprintf(file, "%d ", num_ops);
    }
    //printf("here10\n");
    fprintf(file, "\n");
    //printf("here11\n");
    int denumerator = rand() % 100 + 1;
    int numerator = rand() % 100 + denumerator;
    double coeff = numerator / denumerator;
    //printf("here12\n");
    fprintf(file, "%lf\n", coeff);
    fclose(file);
    //printf("here13\n");
    create_log(&logger, "FINISHED random.c\n", FINISHED, NULL, NULL, 0, cur_time);
    write_log(logger);
    //printf("here14\n");
    int prior = 5;
    write_requests(logger, "src/file1.txt", st, end, prior);
    write_requests(logger, "src/file2.txt", st, end, prior);
    write_requests(logger, "src/file3.txt", st, end, prior);
    write_requests(logger, "src/file4.txt", st, end, prior);
    write_requests(logger, "src/file5.txt", st, end, prior);
    write_requests(logger, "src/file6.txt", st, end, prior);
    write_requests(logger, "src/file7.txt", st, end, prior);
    write_requests(logger, "src/file8.txt", st, end, prior);
    write_requests(logger, "src/file9.txt", st, end, prior);
    //printf("here15\n");
    return 0;
}