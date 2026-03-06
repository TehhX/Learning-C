#include <printUtils.h>

void println(const char *str) {
    printf("%s\n", str);
}

void printVert(const char *str) {
    for (size_t i = 0; str[i] != '\0'; i++)
        printf("%c\n", str[i]);
}
