#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include "headers/lab.h"

void print_menu() {
    printf("|============================|Hello|============================|\n");
    printf("| [Create] Post <Adress>                                        |\n");
    printf("| [Change] Post <Adress>                                        |\n");
    printf("| [Add] <Adress weight id time_create time_get>                 |\n");
    printf("| [Remove] <Id>                                                 |\n");
    printf("| [Table] - print mails of current post                         |\n");
    printf("| [Sort] - to sort mails by index and id                        |\n");
    printf("| [Print expired] - to print expired mails of this post         |\n");
    printf("| [Print non-expired] - to print non-expired mails of this post |\n");
    printf("| [Exit] - to exit the programm                                 |\n");
    printf("|===============================================================|\n");
}

void print_cur_post(Post* current) {
    if (!current) {
        return;
    }
    printf("Current post is located in: %s %s %d %s %d %s\n", current->cur_id->city->data,  current->cur_id->street->data,  current->cur_id->house,  current->cur_id->block->data,  current->cur_id->flat,  current->cur_id->index->data);
}

int main(int argc, char* argv[]) {
    Post* storage_posts = NULL;
    char* arg_one = NULL;
    char* arg_two = NULL;
    My_string* info = NULL;
    status_cmd st_cmd;
    status_code st_activ;
    int capacity_storage = 32;
    int size_storage = 0;
    storage_posts = (Post*)malloc(sizeof(Post*) * (capacity_storage));
    Post* current = NULL;
    Post* tmp_post = NULL;
    Mail* tmp_mail = NULL;
    int tmp_size;
    while (true) {
        print_menu();
        print_cur_post(current);
        st_cmd = command(&arg_one, &arg_two, &info);
        switch (st_cmd) {
            case cmd_exit:
                break;
            case cmd_error_alloc:
                printf("Error alloc detected!!!\n");
                break;
            case cmd_invalid_parameter:
                printf("Invalid parameter detected!!!\n");
                break;
            case cmd_sort:
                qsort(storage_posts, capacity_storage, sizeof(Mail*), compare_mails);
                break;
            case cmd_create_post:
                st_activ = create_post(&current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_activ = add_post_storage(&storage_posts, current, &capacity_storage, &size_storage);
                    if (st_activ == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_activ == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    }
                }
                break;
            case cmd_change_post:
                tmp_post = find_post(storage_posts, info, size_storage);
                if (!tmp_post) {
                    printf("This post doesn`t exist\n");
                } else {
                    current = tmp_post;
                }
                break;
            case cmd_add_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = add_mail(&current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_remove_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = remove_mail(current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                break;
            case cmd_print_expired:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = find_expired_mails(current, &tmp_mail, &tmp_size);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                print_mails(tmp_mail, tmp_size);
                for (int i = 0; i < tmp_size; i++) {
                    free_mail(&tmp_mail[i]);
                }
                free(tmp_mail);
                tmp_mail = NULL;
                break;
            case cmd_print_non_expired:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                st_activ = find_non_expired_mails(current, &tmp_mail, &tmp_size);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                }
                print_mails(tmp_mail, tmp_size);
                for (int i = 0; i < tmp_size; i++) {
                    free_mail(&tmp_mail[i]);
                }
                free(tmp_mail);
                tmp_mail = NULL;
                break;
            case cmd_table:
                if (current) {
                    print_mails(current->mails, current->size);
                } else {
                    printf("Current post is empty!\n");
                }
                break;
        }
        string_clear(info);
        free(info);
        info = NULL;
        if (st_activ == code_error_alloc) {
            break;
        }
        if (st_cmd == cmd_exit || st_cmd == code_error_alloc) {
            break;
        }
    }
    free_storage(&storage_posts, size_storage);
    return 0;
}