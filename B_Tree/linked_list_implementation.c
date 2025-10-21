#include "header.h"

#include "stdlib.h"

struct node;

typedef struct node {
    void *vals;
    struct node *parent;
    struct node **children;
} node;

void b_tree_create(b_tree *const restrict btree, const size_t max_vals, const size_t val_size, a_greater agreater)
{
    btree->max_vals = max_vals;
    btree->val_size = val_size;
    btree->agreater = agreater;

    btree->data = malloc(sizeof(node));
    ((node *) btree->data)->vals = malloc(val_size * max_vals);
    ((node *) btree->data)->parent = NULL;
    ((node *) btree->data)->children = calloc(max_vals, sizeof(node));
}

void b_tree_destroy(b_tree *const restrict btree)
{
    /*
        1 Start at root
        2 If any child node is not null:
            2.1 Go to leftmost child node
            2.2 Goto 2
        3 Else:
            3.1 If parent not null:
                3.1.1 Go up to parent
            3.2 Free last node
            3.3 Goto 2

        Not particularly optimized, or even tested for that matter.
    */

    node *traveller = btree->data; // 1

    for (size_t i = 0; traveller != NULL;)
    {
        // 2
        if (traveller->children[i] != NULL)
        {
            // 2.1
            traveller = traveller->children[i];
            i = 0;
        }
        else if (i == btree->max_vals - 1)
        {
            // 3
            node *const to_free = traveller;

            // 3.1
            if (traveller->parent != NULL)
            {
                traveller = traveller->parent;
                traveller->children[i] = NULL;
            }
            // Hit root node.
            else
            {
                traveller = NULL;
            }

            // 3.2
            free(to_free->vals);
            i = 0;
        }
        else
        {
            ++i;
        }
    }
}

bool b_tree_insert(b_tree *const restrict btree, void *val)
{

}

void b_tree_remove(b_tree *const restrict btree, void *val)
{

}

bool b_tree_has(b_tree *const restrict btree, void *val)
{

}

bool b_tree_write(b_tree const *const restrict btree, FILE *restrict stream)
{

}

bool b_tree_read(b_tree *const restrict btree, FILE *restrict stream)
{

}
