#include "operations.h"

#include <stdio.h>

// Defining .h vars

int five = 5;

const float pi = 3.14159;

int add(int x, int y) {
    return x + y;
}

int subtract(int x, int y) {
    return x - y;
}

int multiply(int x, int y) {
    return x * y;
}

int divide(int x, int y) {
    return x / y;
}

void printThing(const char thing[]) {
    printf("%s\n", thing);
}
