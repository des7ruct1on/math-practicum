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
        end->year = rand() % (2024 - start->year + 1) + start->year; 
        end->month = rand() % 12 + 1; 
        end->day = rand() % 31 + 1; 
        end->hour = rand() % 24; 
        end->minute = rand() % 60; 
        end->second = rand() % 60; 
    }
}

int main(int argc, char* argv[]) {

    FILE* file = fopen("model.txt", "w");
    Logger* logger = NULL;
    my_time cur_time = get_time_now();

    create_log(&logger, "random generation function started\n", STARTED, NULL, NULL, 0, cur_time);
    write_log(logger);
    if (!file) {
        create_log(&logger, "didn`t open file in random.c\n", ERROR_OPPENING, NULL, NULL, 0, cur_time);
        write_log(logger);
        return -1;
    }
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
    my_time st, end;
    get_random_dates(&st, &end);
    fprintf(file, "%d-%d-%dT%d:%d:%d\n", st.year, st.month, st.day, st.hour, st.minute, st.second);
    fprintf(file, "%d-%d-%dT%d:%d:%d\n", end.year, end.month, end.day, end.hour, end.minute, end.second);
    int min_time = rand() % 60 + 1;
    int max_time = min_time + (rand() % (120 - min_time + 1));
    fprintf(file, "%d\n", min_time);
    fprintf(file, "%d\n", max_time);
    int count_posts = rand() %  100 + 1;
    fprintf(file, "%d\n", count_posts);
    for (int i = 0; i < count_posts; i++) {
        int num_ops = rand() % 50 + 10;
        fprintf(file, "%d ", num_ops);
    }
    fprintf(file, "\n");
    int denumerator = rand() % 100;
    int numerator = rand() % 100 + denumerator;
    double coeff = numerator / denumerator;
    fprintf(file, "%lf\n", coeff);
    fclose(file);
    create_log(&logger, "FINISHED random.c\n", FINISHED, NULL, NULL, 0, cur_time);
    write_log(logger);
}