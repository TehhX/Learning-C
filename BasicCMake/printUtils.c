#include <printUtils.h>

void println(const char* P) {
    printf("%s%c", P, '\n');
}

void printVert(const char* P) {
    for (size_t i = 0; P[i] != '\0'; i++)
        printf("%c\n", P[i]);
}
