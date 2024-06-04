#include "Trie.h"

Trie_node_ptr Trie_node_create()
{
    Trie_node_ptr node = (Trie_node_ptr)malloc(sizeof(Trie_node));
    if (!node) return NULL;
    node->is_final = 0;
    for (int i = 0; i < VARIABLE_CHARAS_COUNT; ++i) node->childs[i] = NULL;
    return node;
}

void Trie_node_free(Trie_node_ptr node)
{
    if (!node) return;
    for (int i = 0; i < VARIABLE_CHARAS_COUNT; ++i) Trie_node_free(node->childs[i]);
    free(node);
    node = NULL;
}

status Trie_set_null(Trie_ptr tree)
{
    tree->root = NULL;
    return success;
}

Trie_ptr Trie_create()
{
    Trie_ptr tree = (Trie_ptr)malloc(sizeof(Trie));
    if (!tree) return NULL;
    tree->root = Trie_node_create();
    if (!tree->root)
    {
        free(tree);
        return NULL;
    }
    return tree;
}

void Trie_free(Trie_ptr tree)
{
    Trie_node_free(tree->root);
    free(tree);
    tree = NULL;
}

int find_digits_count_in_number(int number)
{
    if (!number/10) return 1;
    int result = 0;
    while (number > 0)
    {
        number/=10;
        result++;

    }
    return result;
}

status Trie_find(Trie_ptr tree, char * name, Trie_node_ptr * result)
{
    if (!tree) return invalid_function_argument;

    Trie_node_ptr current = tree->root;
    int string_size = strlen(name);
    for (int i = 0; i < string_size; ++i)
    {
        int index = name[i] - '0';
        if (isdigit(name[i])) index = name[i] - '0';
        else if (name[i] == '_') index = VARIABLE_CHARAS_COUNT - 1;
        else index = name[i] - 'A';
        if (!current->childs[index]) return cannot_find;
        current = current->childs[index];
    }
    if (!current->is_final) return cannot_find;
    *result = current;
    return success;
}

status Trie_insert(Trie_ptr tree, char * name, uint32_t value)
{
    if (!tree) return invalid_function_argument;

    int string_size = strlen(name);
    Trie_node_ptr current = tree->root;

    for (int i = 0; i < string_size; ++i)
    {
        int index;
        if (isdigit(name[i])) index = name[i] - '0';
        else if (name[i] == '_') index = VARIABLE_CHARAS_COUNT - 1;
        else index = name[i] - 'A';
        if (!current->childs[index])
        {
            Trie_node_ptr new_node = Trie_node_create();
            if (!new_node) return no_memory;
            current->childs[index] = new_node;
        }
        current = current->childs[index];
    }
    current->value = value;
    current->is_final = 1;
    return success;
}