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
    printf("| [Find] - to find mail by id                                   |\n");
    printf("| [Exit] - to exit the programm                                 |\n");
    printf("|===============================================================|\n");
}

void print_cur_post(Post* current) {
    if (!current) {
        return;
    }
    printf("Current post is located in: %s %s %d %s %d %s\n", current->cur_id->city->data,  current->cur_id->street->data,  current->cur_id->house,  current->cur_id->block->data,  current->cur_id->flat,  current->cur_id->index->data);
}

void print_info_mail(Mail* mail) {
    printf("| Index: %s\n", mail->ad.index->data);
    printf("| City: %s\n", mail->ad.city->data);
    printf("| Street: %s\n", mail->ad.street->data);
    printf("| House: %d\n", mail->ad.house);
    printf("| Block: %s\n", mail->ad.block->data);
    printf("| Flat: %d\n", mail->ad.flat);
    printf("| Weight: %lf\n", mail->weight);
    printf("| Id: %s\n", mail->id->data);
    printf("| Time create: %s\n", mail->time_create->data);
    printf("| Time get: %s\n", mail->time_get->data);
    printf("\n");
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
    Mail* tmp_find = NULL;
    int tmp_size = 32;
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
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                qsort(current->mails, current->size, sizeof(Mail), compare_mails);
                print_mails(current->mails, current->size);
                break;
            case cmd_create_post:
                st_activ = create_post(&current, info);
                if (st_activ == code_error_alloc) {
                    printf("Error alloc detected!!!\n");
                } else if (st_activ == code_invalid_parameter) {
                    printf("Invalid parameter detected!!!\n");
                } else {
                    st_activ = add_post_storage(storage_posts, current, &capacity_storage, &size_storage);
                    if (st_activ == code_error_alloc) {
                        printf("Error alloc detected!!!\n");
                    } else if (st_activ == code_invalid_parameter) {
                        printf("Invalid parameter detected!!!\n");
                    }
                }
                break;
            case cmd_change_post:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
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
                } else {
                    qsort(tmp_mail, tmp_size, sizeof(Mail), compare_mails_date_create);
                    print_mails(tmp_mail, tmp_size);
                    free(tmp_mail);
                    tmp_mail = NULL;
                    tmp_size = 32;
                }
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
                } else {
                    qsort(tmp_mail, tmp_size, sizeof(Mail), compare_mails_date_create);
                    print_mails(tmp_mail, tmp_size);
                    free(tmp_mail);
                    tmp_mail = NULL;
                    tmp_size = 32;
                }
                break;
            case cmd_table:
                if (current) {
                    print_mails(current->mails, current->size);
                } else {
                    printf("Current post is empty!\n");
                }
                break;
            case cmd_find_mail:
                if (!current) {
                    printf("Current post is empty!\n");
                    break;
                }
                if (!is_sorted(current->mails, current->size)) {
                    qsort(current->mails, current->size, sizeof(Mail), compare_mails_id);
                }
                tmp_find = find_mail(current, *info, current->size);
                if (!tmp_find) {
                    printf("This mail does not exist in this post!\n");
                } else {
                    print_info_mail(tmp_find);
                    tmp_find = NULL;
                }
                break;
            default:
                break;
        }
        string_clear(info);
        free(info);
        info = NULL;
        if (st_cmd == cmd_error_alloc) {
            break;
        }
        if (st_activ == code_error_alloc) {
            break;
        }
        if (st_cmd == cmd_exit) {
            break;
        }
    }
    free_storage(storage_posts, size_storage);
    free(storage_posts);
    return 0;
}