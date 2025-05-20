#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int value1;
    double value2;
} Thing;

void printThingCopy(Thing t) {
    printf("(%d, %lf)\n", t.value1, t.value2);
}

void printThingPtr(Thing* t) {
    printf("(%d, %lf)\n", t->value1, t->value2);
}

int main() {
    // Standard stuff
    Thing t1 = { 10, 12.5 };
    printThingCopy(t1);
    printThingPtr(&t1);
/*
    Compound literals
    Regular Comp. Lit: (type){ initializer }
    Or for a pointer: &(type){ initializer }
    Both only exist within function scope (i think)
*/
    printThingCopy((Thing){ 12, 3.14 });
    printThingPtr(&(Thing){ 14, 6.87 });
}
