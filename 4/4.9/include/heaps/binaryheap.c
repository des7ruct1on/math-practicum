#include "headers/binaryheap.h"

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
        swap_request(&q->data[index_last], &q->data[(index_last - 1) / 2]);
        index_last = (index_last - 1) / 2;

    }
}


status_code insert_bh(Binary_heap* q, Request value) {
    if (!q) return;
    q->data[q->size] = value;
    q->size++;
    if (q->size + 1 == q->capacity) {
        q->capacity *= 2;
        Request* check = realloc(q->data, sizeof(Request) * q->capacity);
        if (!check) {
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
            swap_request(&q->data[i], &q->data[i * 2 + 1]);
            i = i * 2 + 1;
        } else {
            swap_request(&q->data[i], &q->data[i * 2 + 2]);
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
    swap_request(&q->data[0], &q->data[q->size - 1]);
    q->size--;
    heapify_down_bh(q);
    return value;
}

void print_heap_bh(Binary_heap* h, int index, int level) {
    if (!h || index >= h->size) return;

    print_heap_bh(h, 2 * index + 2, level + 1);

    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%d\n", h->data[index]);

    print_heap_bh(h, 2 * index + 1, level + 1);
}

status_code merge_destruction_bh(Binary_heap* a, Binary_heap* b) {
    if (!b) return code_success;
    status_code st_act;
    while (b->size) {
        st_act = insert_bh(a, *pop_bh(b));
        if (st_act != code_success) return st_act;
    }
    return code_success;
}

status_code merge_no_destruction_bh(Binary_heap* a, Binary_heap* b) {
    if (!b) return code_success;
    Binary_heap* tmp = NULL;
    status_code st_act;
    st_act = create_queue_bh(&tmp);
    if (st_act != code_success) return st_act;
    while (b->size) {
        Request* c = pop_bh(b);
        Request* tmp_c;
        st_act = request_copy(*c, &tmp_c);
        if (st_act != code_success) return st_act;
        insert_bh(a, *c);
        insert_bh(tmp, *tmp_c);
    }
    return merge_destruction_bh(b, tmp);

}

void free_binary(Binary_heap*  heap) {
    if (!heap) return;
    for (int i = 0; i < heap->size; i++) {
        free(heap->data->id);
        free(heap->data->text);
    }
    fre(heap->data);
    free(heap);
}