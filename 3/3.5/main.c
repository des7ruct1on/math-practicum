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
    printf("\n|==============================| Hello, user! |==============================|\n");
    printf("| Enter `Sort` to sort storage with students                                 |\n");
    printf("| Enter `Find` to find student and print info about him in <OUTPUT_FILE>     |\n");
    printf("| Enter `Table` to print table od students                                   |\n");
    printf("| Enter `Rewrite` to rewrite <INPUT_FILE>                                    |\n");
    printf("| Enter `EXIT` to exit                                                       |\n");
    printf("|============================================================================|\n");
}
void print_menu_find() {
    printf("\n|============================================================================|\n");
    printf("| Enter `Id` to find by id                                                   |\n");
    printf("| Enter `Name` to find by name                                               |\n");
    printf("| Enter `Surname` to find by surname                                         |\n");
    printf("| Enter `Group` to find bu group                                             |\n");
    printf("|============================================================================|\n");
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Your programm should start at least with: %s <FILE_IN>\n", argv[0]);
        return -1;
    }
    const char* file_in = argv[1];
    const char* file_out = argv[2];
    FILE* in = fopen(file_in, "r");
    if (!in) {
        printf("Can`t open file!!!\n");
        return -2;
    }
    FILE* out;
    bool able_to_rewrite = false;
    if (file_out != NULL) {
        out = fopen(file_out, "w");
        if (!out) {
            printf("Can`t open file!!!\n");
            fclose(in);
            return -3;
        }
        able_to_rewrite = true;
    } else {
        able_to_rewrite = false;
    }
    Student* storage = NULL;
    int capacity;
    status_code st_read = read_from_file(in, &storage, &capacity);
    if (st_read == code_invalid_parameter) {
        printf("Invalid parameter detected!!!\n");
        free(storage);
        fclose(in);
        fclose(out);
    } else if (st_read == code_error_alloc) {
        printf("Error alloc detected!!!\n");
        free(storage);
        fclose(in);
        fclose(out);
    }
    bool is_sorted_storage = is_sorted(storage, capacity);
    char choose[STR_SIZE];
    char type_find[STR_SIZE];
    char find_arg[STR_SIZE];
    unsigned int find_id;
    int index_cur, index_tmp;
    while (true) {
        print_menu();
        scanf("%s", choose);
        if (!strcmp(choose, "Sort")) {
            qsort(storage, capacity, sizeof(Student), compare_students);
            continue;
        } else if (!strcmp(choose, "Find")) {
            if (!is_sorted) {
                printf("Your table must be sorted!\n");
                continue;
            } else {
                void print_menu_find();
                scanf("%s", type_find);
                printf("Enter parameter: ");
                if (!strcmp(type_find, "Id")) {
                    scanf("%d", &find_id);
                    Student* find = get_student_id(storage, capacity, &index_cur, find_id);
                    if (!find) {
                        printf("This student does not exist!\n");
                        continue;
                    }
                    if (!able_to_rewrite) {
                        print_student_chars(stdout, find);
                    } else {
                        print_student_chars(out, find);
                    }
                    continue;
                } else if (!strcmp(type_find, "Surname")) {
                    scanf("%s", find_arg);
                    Student* find = get_student_id(storage, capacity, &index_cur, find_id);
                    if (!find) {
                        printf("This student does not exist!\n");
                        continue;
                    }
                    
                } else if (!strcmp(type_find, "Name")) {
                    scanf("%s", find_arg);
                    
                } else if (!strcmp(type_find, "Group")) {
                    scanf("%s", find_arg);
                    
                }

            }
            continue;
        } else if (!strcmp(choose, "Table")) {
            continue;
        } else if (!strcmp(choose, "Rewrite")) {
            continue;
        } else if (!strcmp(choose, "Exit")) {
            continue;
        }
    }
    fclose(in);
    fclose(out);
    free(storage);

    return 0;
}