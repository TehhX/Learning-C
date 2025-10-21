#ifndef B_TREE_H
#define B_TREE_H

#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"

typedef bool (*a_greater)(void *val_a, void *val_b);

// General purpose definition for multiple implementations
typedef struct b_tree {
    void *data; // Obfuscated, will decide type depending on implementation.
    size_t max_vals;
    size_t val_size;
    a_greater agreater; // For deciding if val_a is greater than val_b
} b_tree;

// Creates a b_tree based on arguments.
void b_tree_create(b_tree *const restrict btree, const size_t max_vals, const size_t val_size, a_greater);

// Frees memory associated with the b_tree.
void b_tree_destroy(b_tree *const restrict btree);

// Inserts *val into btree, returns true if it already exists.
bool b_tree_insert(b_tree *const restrict btree, void *val);

// Removes *val from btree.
void b_tree_remove(b_tree *const restrict btree, void *val);

// Returns true if btree contains *val.
bool b_tree_has(b_tree *const restrict btree, void *val);

// Writes a btree to stream, returns false if failed.
bool b_tree_write(b_tree const *const restrict btree, FILE *restrict stream); 

// Reads a btree from stream, returns false if failed.
bool b_tree_read(b_tree *const restrict btree, FILE *restrict stream); 

#endif // B_TREE_H
