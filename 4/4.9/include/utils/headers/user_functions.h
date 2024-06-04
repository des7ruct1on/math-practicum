#ifndef user_functions_h
#define user_functions_h

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "struct.h"
#include "logger.h"
#include "../../heaps/headers/binaryheap.h"
#include "../../heaps/headers/binomial.h"
#include "../../heaps/headers/fib.h"
#include "../../heaps/headers/leftistheap.h"
#include "../../heaps/headers/skew.h"
#include "../../heaps/headers/treap.h"
#include "../../maps/headers/array.h"
#include "../../maps/headers/bst.h"
#include "../../maps/headers/hashset.h"
#include "../../maps/headers/trie.h"

void print_info(Post** posts, int size, Model* model);
void free_post(Post* post, Heap type_heap);
status_code create_post(Logger* logger, Post** post, Heap type_heap, int _size_op, Model* model);
Heap get_type_heap(char* str);
Storage get_type_map(char* str);
status_code get_full_model(Logger* logger, Model** model, const char* filename, Post*** posts_nodes, int* size_posts);
status_code get_full_model_from_user(Logger* logger, Model** model, Post*** posts_nodes, int* size_posts);
status_code create_map(Logger* logger, Storage type_storage, Model* model);
status_code read_request(Logger* logger, int count_files, char* filenames[], Request** reqs, int* total_reqs, int choose);
status_code get_all_info(Logger* logger, int choose, int argc, char* argv[], Model** model, Request** reqs, Post*** posts_nodes, int* size_posts, int* count_reqs);
status_code insert_map(Logger* logger, Model* model, Request to_add, Post** posts, int count_posts, int* index_post, my_time _time);
Post* find_less_load(Post** posts, int size);
void print_choose();
void process_requests(Logger* logger, Model* model, Post** posts, my_time _time);
char* int_to_string(int number);
void print_time(my_time a);
#endif