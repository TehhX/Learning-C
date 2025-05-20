#include <stdio.h>

int main() {
    // Array of floats
    float fArr[] = { 1.0f, 0.5f, 0.25f, 0.125f, 0.0625f };

    // Array of integers
    int iArr[10];

    for (size_t i = 0; i < 10; i++)
        iArr[i] = i * 5;

    for (size_t i = 0; i < 10; i++)
        printf("Value %zu: %d\n", i, iArr[i]);
    printf("\n");

    // MultiD array of doubles
    double mDDArr[2][5] = { { 2.0, 2.5, 3.0, 3.5, 4.0 }, { -1.5, -2.0, -2.5, -3.0, -3.5 } };

    for (size_t x = 0; x < 2; x++)
        for (size_t y = 0; y < 5; y++)
            printf("(%zu, %zu) = %lf\n", x, y, mDDArr[x][y]);
}
