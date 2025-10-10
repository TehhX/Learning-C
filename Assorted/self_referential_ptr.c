// The void pointer points to itself. This means that both its address and value are the same. Dereferencing it will give it the same value, and can be repeated ad infinitum.

#include "stdio.h"

int main()
{
    void *ptr;
    ptr = &ptr;

    while (1)
    {
        // Assign ptr to its dereferenced value, print it, repeat (Learned the format-spec for a pointer as well)
        printf("%p\n", ptr = *(void **) ptr); // 
    }
}
