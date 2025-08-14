#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// May take longer to run a given program, but will run checks on all matrix operations if defined.
#if 1
    #define MATRIX_DEBUG_MISC(COND, WARN, EXITS) if (COND) { puts(WARN); if (EXITS) abort(); }
    #define MATRIX_DEBUG_INPTR(COND, NAME) if (COND) { puts("NULL MATRIX POINTER IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_VALUS(COND, NAME) if (COND) { puts("NULL VALUES MATRIX POINTER IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_WIDHE(INPTR_1, INPTR_2, NAME) if (INPTR_1->width != INPTR_2->width || INPTR_1->height != INPTR_2->height) { puts("WIDTH/HEIGHT MISMATCH IN \"" NAME "\"!"); abort(); }
#else
    #define MATRIX_DEBUG_MISC  ;
    #define MATRIX_DEBUG_INPTR ;
    #define MATRIX_DEBUG_VALUS ;
    #define MATRIX_DEBUG_WIDHE ;
#endif

// Contains values, width/height of a mathematical matrix.
typedef struct matrix_t {
    int width;
    int height;
    int *values;
} matrix_t;

// Returns the value at an (x, y) position of a mathematical matrix.
static inline int matrix_get(const matrix_t *m, int x, int y) {
    MATRIX_DEBUG_INPTR(!m, "matrix_get");
    MATRIX_DEBUG_MISC(x < 0 || y < 0, "NEGATIVE CO-ORDS IN \"matrix_get\"!", 1);

    return m->values[x + y * m->width];
}

// Returns a matrix from width, height, and values.
matrix_t matrix_create(int width, int height, int values[]) {
    MATRIX_DEBUG_MISC(!values, "BAD VALUES IN \"matrix_create\"!", 1);
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD MATRIX WIDTH/HEIGHT IN \"matrix_create\"!", 1);

    matrix_t matrix = { .width = width, .height = height };
    matrix.values = malloc(sizeof(int) * width * height);
    memcpy(matrix.values, values, sizeof(int) * width * height);

    return matrix;
}

// Frees the memory used by a matrix.
static inline void matrix_destroy(matrix_t *m) {
    MATRIX_DEBUG_INPTR(!m, "matrix_destroy");
    MATRIX_DEBUG_VALUS(!m->values, "matrix_destroy");

    free(m->values);
}

// Renders a matrix to stream.
void matrix_fprint(FILE *stream, const matrix_t *m) {
    MATRIX_DEBUG_INPTR(!m, "matrix_fprint");
    MATRIX_DEBUG_VALUS(!m->values, "matrix_fprint");
    MATRIX_DEBUG_MISC(!stream, "NULL FILE POINTER IN \"matrix_fprint\"!", 1);

    for (int y = 0; y < m->height; ++y) {
        for (int x = 0; x < m->width; ++x)
            fprintf(stream, "%d ", matrix_get(m, x, y));
        putc('\n', stream);
    }
}

// Renders a matrix to stdout.
static inline void matrix_print(const matrix_t *m) {
    matrix_fprint(stdout, m);
}

// Returns the sum of two matrices.
matrix_t matrix_add(const matrix_t *addend_1, const matrix_t *addend_2) {
    MATRIX_DEBUG_WIDHE(addend_1, addend_2, "matrix_add");
    MATRIX_DEBUG_INPTR(!addend_1 || !addend_2, "matrix_add");
    MATRIX_DEBUG_VALUS(!addend_1->values || !addend_2->values, "matrix_add");

    matrix_t sum = { .width = addend_1->width, .height = addend_1->height };

    const int valuesSize = sizeof(int) * sum.width * sum.height;
    sum.values = malloc(valuesSize);
    memcpy(sum.values, addend_1->values, valuesSize);

    for (int i = 0; i < valuesSize / sizeof(int); ++i)
        sum.values[i] += addend_2->values[i];

    return sum;
}

// Returns the difference of two matrices.
matrix_t matrix_sub(const matrix_t *minuend, const matrix_t *subtrahend) {
    MATRIX_DEBUG_WIDHE(minuend, subtrahend, "matrix_sub");
    MATRIX_DEBUG_INPTR(!minuend || !subtrahend, "matrix_sub");
    MATRIX_DEBUG_VALUS(!minuend->values || !subtrahend->values, "matrix_sub");

    matrix_t difference = { .width = minuend->width, .height = minuend->height };

    const int valuesSize = sizeof(int) * difference.width * difference.height;
    difference.values = malloc(valuesSize);
    memcpy(difference.values, minuend->values, valuesSize);

    for (int i = 0; i < valuesSize / sizeof(int); ++i)
        difference.values[i] -= subtrahend->values[i];

    return difference;
}

// Returns the product of two matrices.
matrix_t matrix_mult(const matrix_t *multiplicand, const matrix_t *multiplier) {
    // TODO: Debugs.

    matrix_t product;

    // TODO: Make it work.

    puts("CANNOT MULTIPLY YET!");
    exit(EXIT_FAILURE);
}

// For testing matrix functionality.
int main() {
    matrix_t first = matrix_create(2, 2, (int[]) { 1, 2, 3, 4 });
    matrix_t second = matrix_create(2, 2, (int[]) { 25, 50, 75, 100 });

    // Print first
    matrix_print(&first);
    putc('\n', stdout);

    // Print second
    matrix_print(&second);
    putc('\n', stdout);

    // Print sum of first and second
    matrix_t sum = matrix_add(&first, &second);
    matrix_print(&sum);
    putc('\n', stdout);

    // Print difference of first and second
    matrix_t difference = matrix_sub(&first, &second);
    matrix_print(&difference);
    putc('\n', stdout);

    // Print product of first and second
    matrix_t product = matrix_mult(&first, &second);
    matrix_print(&product);
    putc('\n', stdout);

    // Clean up
    matrix_destroy(&first);
    matrix_destroy(&second);
    matrix_destroy(&sum);
    matrix_destroy(&difference);
    matrix_destroy(&product);
}
