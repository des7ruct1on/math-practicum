#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/tree.h"

void print_menu() {
    printf("\n");
    printf("[First] <val> - to print first N common words\n");
    printf("[Len] <Max / Min> - to print word with max / min len\n");
    printf("[Print] <FILE> - to print tree in file\n");
    printf("[Stats] - to print stats about words\n");
    printf("[Find] <Word> - to find exact word and print info about\n");
    printf("[Exit] - to exit\n");
    printf("[Depth] - get depth of tree\n");
    printf("[Get] <File> - get tree from file\n");
    printf("\n");
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Your programm must start with: %s <FILE_IN>\n", argv[0]);
        return -1;
    }
    Node* Tree = NULL;
    List_tree* list = NULL;
    status_cmd st_decision;
    status_code st_activ = read_from_file(argv, argc, &Tree, &list);
    if (st_activ == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        return -2;
    } else if (st_activ == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        return -3;
    } else if (st_activ == code_error_oppening) {
        printf("Can`t open file!!!\n");
        return -4;
    }

    char* arg_one = NULL;
    int num;
    Node* find = NULL;
    FILE* out;
    while (true) {
        print_menu();
        st_decision = command(&arg_one);
        switch (st_decision) {
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            case cmd_first:
                num = atoi(arg_one);
                print_common_words(list, num);
                break;
            case cmd_len:
                if (!strcmp(arg_one, "Max")) {
                    print_max_len_word(list);
                } else if (!strcmp(arg_one, "Min")) {
                    print_least_len_word(list);
                } else {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_find:
                find = search_in_tree(Tree, arg_one);
                if (find) {
                    printf("word: %s, met %d times\n", find->word, find->count);
                } else {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_depth:
                printf("Depth: %d\n", find_depth(Tree));
                break;
            case cmd_stat:
                print_stats(Tree);
                break;
            case cmd_get_file:
                out = fopen(arg_one, "r");
                printf("111\n");
                if (!out) {
                    printf("Can`t open file!!!\n");
                    break;
                }
                st_activ = get_tree_from_file(out, &Tree, &list);
                fclose(out);
                break;
            case cmd_print:
                out = fopen(arg_one, "w");
                if (!out) {
                    printf("Can`t open file!!!\n");
                    break;
                }
                write_to_file(out, Tree, 0);
                fclose(out);
                break;
            case cmd_exit:
                break;
        }
        if (arg_one) {
            free(arg_one);
            arg_one = NULL;
        }
        if (st_decision == cmd_exit || st_activ == code_error_alloc) {
            break;
        }
    }
    free_tree(Tree);
    destroy_list(list);
    return 0;
}