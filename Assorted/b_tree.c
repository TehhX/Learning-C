#include "stdlib.h"
#include "stdbool.h"

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

// Creates a b_tree based on arguments.
void b_tree_create(b_tree *const restrict btree, const size_t max_vals, const size_t val_size)
{
    btree->max_vals = max_vals;
    btree->val_size = val_size;

    btree->root = malloc(sizeof(node));
    btree->root->vals = malloc(sizeof(void *) * max_vals);
    btree->root->parent = 0;
    btree->root->children = calloc(sizeof(node), max_vals);
}

// Frees memory associated with the b_tree.
void b_tree_destroy(b_tree *const restrict btree)
{
    
}

// Inserts *val into btree, returns true if it already exists.
bool b_tree_insert(b_tree *const restrict btree, void *val)
{

}

// Removes *val from btree.
void b_tree_remove(b_tree *const restrict btree, void *val)
{

}

// Returns true if btree contains *val.
bool b_tree_has(b_tree *const restrict btree, void *val)
{

}

// Demonstrating usage of b_tree.
int main()
{
    b_tree btree;
    b_tree_create(&btree, 4, sizeof(int));

    b_tree_destroy(&btree);
}
