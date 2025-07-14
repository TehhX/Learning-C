#include "stdio.h"

// Function pointer notation:
// <return type> (*<const if desired> <name>)(<parameters>)
void takesCallback(int (*getNum)(int, int)) {
    const int i =  2,
              j = 16,
              k = getNum(i, j);

    printf("%d\n", i * j * k);
}

int more(int a, int b) {
    return a > b ? a : b;
}

int less(int a, int b) {
    return a < b ? a : b;
}

int first(int a, int b) {
    return a;
}

int last(int a, int b) {
    return b;
}

int main() {
    takesCallback(less);
    takesCallback(more);

    takesCallback(first);
    takesCallback(last);

    // Local pointer variable
    int (*myFunc)(int, int) = first;
    takesCallback(myFunc);

    myFunc = last;
    takesCallback(myFunc);

    int (*const immutableFuncPtr)(int, int) = more;
    takesCallback(immutableFuncPtr);

    // Const fptr cannot be assigned, like other const types. The following won't compile for this reason:
    // immutableFuncPtr = less;
}
