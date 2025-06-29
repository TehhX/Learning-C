#include <stdio.h>

int main() {
    int val = 1928384756;
    printf("   Regular Value: %15d\n", val);

    int *a = &val;
    printf("Memory address A: %15llu\n", (unsigned long long) a);

    int **b = &a;
    printf("Memory address B: %15llu\n", (unsigned long long) b);

    int ***c = &b;
    printf("Memory address C: %15llu\n", (unsigned long long) c);

    void *d = &c; // Actual type is int****. Void simply obscures it.
    printf(" 4x deref. value: %15d\n", ****(int****) d); // Convert a void pointer to underlying type, dereference back into integer
}
