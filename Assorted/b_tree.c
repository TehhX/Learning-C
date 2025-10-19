#include "stdlib.h"

typedef struct node node;

struct node {
    void **vals;
    node *parent;
    node **children;
};

typedef struct b_tree {
    node *root;
    size_t max_vals;
    size_t val_size;
} b_tree;

void b_tree_create(b_tree *const restrict btree, const size_t max_vals, const size_t val_size)
{
    btree->max_vals = max_vals;
    btree->val_size = val_size;

    btree->root = malloc(sizeof(node));
    btree->root->vals = malloc(sizeof(void *) * max_vals);
    btree->root->parent = 0;
    btree->root->children = calloc(sizeof(node), max_vals);
}

void b_tree_destroy(b_tree *const restrict btree)
{

}

int main()
{
    b_tree btree;
    b_tree_create(&btree, 4, sizeof(int));

    b_tree_destroy(&btree);
}
