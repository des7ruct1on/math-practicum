#include "headers/trie.h"


status_code create_trie(Trie** trie) {
    if (*trie) return code_invalid_parameter;

    *trie = (Trie*)malloc(sizeof(Trie));
    if (!*trie) {
        return code_error_alloc;
    }
    (*trie)->size = 0;
    for (int i = 0; i < 10; i++) {
        (*trie)->nodes[i] = NULL;
    }
    return code_success;
}

status_code insert_trie(Trie* root, char* key, Post* _office) {
    int len = strlen(key);
    Trie* cur = root;
    status_code st_act;
    for (int i = 0; i < len; i++) {
        int ind = key[i] - '0';
        if (!cur->nodes[ind]) {
            st_act = create_trie(&cur->nodes[ind]);
            if (st_act != code_success) {
                return st_act;
            }

        }
        cur = cur->nodes[ind];
    }
    cur->size++;
    cur->office = _office;
    return code_success;
}
/*
char str_[256];
int check = 0;



void write_trie(Trie* root) {
    if (!root) return;
    
    if (root->size > 0) {
        printf("%s\n", str_);
    }
    
    for (int i = 0; i < 10; i++) {
        if (root->nodes[i]) {
            str_[check] = '0' + i; 
            check++;
            write_trie(root->nodes[i]);
            check--;
            str_[check] = '\0';
        }
    }
}
*/

Post* search_trie(Trie* root, char* key) {
    if (!root || !key) return NULL;
    Trie* cur = root;
    int len = strlen(key);
    for (int i = 0; i < len; i++) {
        cur = cur->nodes[key[i] - '0'];
        if (!cur) {
            return NULL;
        }
    }
    if (cur && cur->size) return cur->office;
    return NULL;
}

bool has(Trie* root, const char* str) {
    Trie* cur = root;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        cur = cur->nodes[str[i] - 'a'];
        if (!cur) {
            return false;
        }
    }

    return cur->size > 0;
}

void free_trie(Trie* trie, void(*free_storage)(void*)) {
    if (!trie) return;
    for (int i = 0; i < 10; i++) {
        if (trie->nodes[i]) {
            free_trie(trie->nodes[i], free_storage);
            trie->nodes[i] = NULL;
        }
    }
    if (trie->size) {
        free_storage(trie->office->storage);
        free(trie->office->ops);
    }
    free(trie);
}
/*
int main(int argc, char* argv[]) {
    Trie* trr = NULL;
    status_code st_act;
    st_act = create_trie(&trr);
    st_act = insert_trie(trr, "123");
    st_act = insert_trie(trr, "1234565");
    st_act = insert_trie(trr, "3456");
    st_act = insert_trie(trr, "234542");
    st_act = insert_trie(trr, "5345345");
    st_act = insert_trie(trr, "5345");
    st_act = insert_trie(trr, "777777");
    st_act = insert_trie(trr, "0");

    write_trie(trr);
}
*/