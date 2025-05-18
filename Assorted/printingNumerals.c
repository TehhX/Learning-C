#include <stdio.h>

int main() {
    // printf will print what is in the first string, replacing %<type> with the next arguments
    // specified. It will then continue printing the string.

    // %d denotes int
    int a = 15;
    printf("Number: %d\n", a);

    // %f denotes float
    float b = 3.14;
    printf("Pi: %f\n", b);

    // %lf denotes long float, aka double
    double c = 123.456;
    printf("Sequential: %lf\n", c);
}
