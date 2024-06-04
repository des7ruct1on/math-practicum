#include "headers/binaryheap.h"
#include <math.h>

int binary_heap_size(Binary_heap* heap) {
    return heap->size;
}

status_code create_queue_bh(Binary_heap** q) {
    (*q) = (Binary_heap*)malloc(sizeof(Binary_heap));
    if (!(*q)) return code_error_alloc;
    (*q)->size = 0;
    (*q)->capacity = 32;
    (*q)->data = (Request*)malloc(sizeof(Request) * (*q)->capacity);
    if (!(*q)->data ) return code_error_alloc;
    return code_success;
}

void heapify_up_bh(Binary_heap* q) {
    if (!q) return;
    int index_last = q->size - 1;
    while (q->data[(index_last - 1) / 2].priority < q->data[index_last].priority && index_last >= 0) {
        swap_request(&(q->data[index_last]), &(q->data[(index_last - 1) / 2]));
        index_last = (index_last - 1) / 2;

    }
}


status_code insert_bh(Logger* logger, Binary_heap* q, Request value) {
    if (!q) return code_invalid_parameter;
    q->data[q->size] = value;
    q->size++;
    if (q->size + 1 == q->capacity) {
        q->capacity *= 2;
        Request* check = realloc(q->data, sizeof(Request) * q->capacity);
        if (!check) {
            create_log(&logger, "didnt realloc while inserting\n", BAD_ALLOC, NULL, NULL, 0, get_time_now());
            write_log(logger);
            q->capacity /= 2;
            return code_error_alloc;
        } else {
            q->data = check;
        }
    }
    heapify_up_bh(q);
    return code_success;
}

void heapify_down_bh(Binary_heap* q) {
    if (!q) return;
    int i = 0;
    while (i < q->size) {
        int cur_index = i;
        int left_index = i * 2 + 1;
        int right_index = i * 2 + 2;
        if (left_index >= q->size || right_index >= q->size) {
            break;
        }
        if (q->data[left_index].priority > q->data[right_index].priority) {
            swap_request(&(q->data[i]), &(q->data[i * 2 + 1]));
            i = i * 2 + 1;
        } else {
            swap_request(&(q->data[i]), &(q->data[i * 2 + 2]));
            i = i * 2 + 2;
        }
        if (i >= q->size) {
            break;
        }
        if (cur_index == i) {
            break;
        }
    }
}

Request* pop_bh(Binary_heap* q) {
    if (!q) return NULL;
    Request* value = &q->data[0];
    swap_request(&(q->data[0]), &(q->data[q->size - 1]));
    printf("sdelal swap\n");
    q->size--;
    heapify_down_bh(q);
    printf("ushel\n");
    return value;
}

status_code merge_destruction_bh(Logger * logger, Binary_heap* a, Binary_heap* b) {
    if (!b) return code_success;
    status_code st_act;
    while (b->size > 0) {
        Request* t = pop_bh(b);
        int _size = b->size;
        //printf("%d\n", _size);
        st_act = insert_bh(logger, a, *t);
        if (st_act != code_success) {
            create_log(&logger, "error after insert, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            return st_act;
        }
        if (_size == 0) break;
    }
    return code_success;
}

status_code merge_no_destruction_bh(Logger* logger, Binary_heap* a, Binary_heap* b) {
    if (!b) return code_success;
    Binary_heap* tmp = NULL;
    status_code st_act;
    st_act = create_queue_bh(&tmp);
    if (st_act != code_success) {
        create_log(&logger, "error after insert, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
        write_log(logger);
        return st_act;
    }
    while (b->size) {
        Request* c = pop_bh(b);
        Request* tmp_c;
        st_act = request_copy(*c, &tmp_c);
        if (st_act != code_success) {
            create_log(&logger, "error after copying request\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            return st_act;
        }
        st_act = insert_bh(logger, a, *c);
        if (st_act != code_success) {
            create_log(&logger, "error after insert, checl logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            return st_act;
        }
        st_act = insert_bh(logger, tmp, *tmp_c);
        if (st_act != code_success) {
            create_log(&logger, "error after insert, check logs\n", get_sev_from_status(st_act), NULL, NULL, 0, get_time_now());
            write_log(logger);
            return st_act;
        }
    }
    return merge_destruction_bh(logger, b, tmp);

}

void free_binary(Binary_heap*  heap) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++) {
        free(heap->data->id);
        free(heap->data->text);
    }
    free(heap->data);
    free(heap);
}