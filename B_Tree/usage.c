#include "header.h"

#include "stdio.h"

bool int_is_greater(void *val_a, void *val_b)
{
    return (*(int *)val_a) > (*(int *)val_b);
}

int main()
{
    b_tree btree;
    b_tree_create(&btree, 4, sizeof(int), int_is_greater);

    for (int i = 0; i < 100; ++i)
    {
        int input = i * 2;
        b_tree_insert(&btree, &input);
    }

    b_tree_destroy(&btree);

    printf("End of %s::main()::%d reached.\n", __FILE__, __LINE__);
}
