#include "operations.h"

#include <stdio.h>

int main() {
    printf("Add: %d\n", add(pi, 5));
    printf("Add again: %d\n", add(euler, 3));
    printf("Subtract: %d\n", subtract(0, 5));
    printf("Multiply: %d\n", multiply(add(1, 2), subtract(5, 2)));
    printf("Divide: %d\n", divide(15, 3));
    printThing("Thing to print.");
}
