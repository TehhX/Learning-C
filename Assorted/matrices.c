#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// The decimal precision of matrix printed doubles. Define when compiling to override the default value of 2 places.
#ifndef MATRIX_PRINT_DEC_PLACES
#define MATRIX_PRINT_DEC_PLACES "2"
#endif

// May take longer to run a given program, but will run checks on all matrix operations if defined during compilation.
// MATRIX_DEBUG_INLINE will make a function static inline only when debug macro is NOT defined. For when a function is usually small, but debug statements would make it larger.
#ifdef MATRIX_DEBUG
    #define MATRIX_DEBUG_MISC(COND, WARN, EXITS) if (COND) { puts(WARN); if (EXITS) abort(); }
    #define MATRIX_DEBUG_INPTR(PTR, NAME) if (!PTR) { puts("NULL MATRIX POINTER IN \"" NAME "\"!"); abort(); } else if (!PTR->values) { puts("NULL MATRIX POINTER VALUES IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_WIDHE(INPTR_1, INPTR_2, NAME) if (INPTR_1->width != INPTR_2->width || INPTR_1->height != INPTR_2->height) { puts("WIDTH/HEIGHT MISMATCH IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_INLINE
#else
    #define MATRIX_DEBUG_MISC(X, Y, Z)  ;
    #define MATRIX_DEBUG_INPTR(X, Y)    ;
    #define MATRIX_DEBUG_WIDHE(X, Y, Z) ;
    #define MATRIX_DEBUG_INLINE static inline
#endif

// Contains values, width/height of a mathematical matrix.
typedef struct matrix_t {
    int width;
    int height;
    double *values;
} matrix_t;

// Returns the value at an (x, y) position of a mathematical matrix. Value is a mutable pointer.
MATRIX_DEBUG_INLINE double *matrix_get_mut(matrix_t *m, const int x, const int y) {
    MATRIX_DEBUG_INPTR(m, "matrix_get_mut");
    MATRIX_DEBUG_MISC(x < 0 || y < 0, "NEGATIVE CO-ORDS IN \"matrix_get_mut\"!", 1);

    return m->values + x + y * m->width;
}

// Returns the value at an (x, y) position of a mathematical matrix. Value is immutable.
MATRIX_DEBUG_INLINE double matrix_get(const matrix_t *m, const int x, const int y) {
    MATRIX_DEBUG_INPTR(m, "matrix_get");
    MATRIX_DEBUG_MISC(x < 0 || y < 0, "NEGATIVE CO-ORDS IN \"matrix_get\"!", 1);

    return m->values[x + y * m->width];
}

// Creates a matrix without ininitalizing zeros or values.
matrix_t matrix_create_uninitialized(const int width, const int height) {
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD WIDTH/HEIGHT IN \"matrix_create_uninitialized\"!", 1);

    matrix_t matrix = { .width = width, .height = height };
    matrix.values = malloc(sizeof(double) * width * height);

    return matrix;
}

// Creates a matrix with all zeros.
matrix_t matrix_create_initialized(const int width, const int height) {
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD WIDTH/HEIGHT IN \"matrix_create_initialized\"!", 1);

    matrix_t matrix = { .width = width, .height = height };
    matrix.values = calloc(width * height, sizeof(double));

    return matrix;
}

// Returns a matrix from width, height, and values.
matrix_t matrix_create_vals(const int width, const int height, const double values[]) {
    MATRIX_DEBUG_MISC(!values, "BAD VALUES IN \"matrix_create_vals\"!", 1);
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD MATRIX WIDTH/HEIGHT IN \"matrix_create_vals\"!", 1);

    matrix_t matrix = matrix_create_uninitialized(width, height);
    memcpy(matrix.values, values, sizeof(double) * width * height);

    return matrix;
}

// Frees the memory used by a matrix.
MATRIX_DEBUG_INLINE void matrix_destroy(matrix_t *m) {
    MATRIX_DEBUG_INPTR(m, "matrix_destroy");

    free(m->values);
}

// Renders a matrix to stream.
void matrix_fprint(FILE *stream, const matrix_t *m) {
    MATRIX_DEBUG_INPTR(m, "matrix_fprint");
    MATRIX_DEBUG_MISC(!stream, "NULL FILE POINTER IN \"matrix_fprint\"!", 1);

    for (int y = 0; y < m->height; ++y) {
        for (int x = 0; x < m->width; ++x)
            fprintf(stream, "%0." MATRIX_PRINT_DEC_PLACES "lf ", matrix_get(m, x, y));
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
    MATRIX_DEBUG_INPTR(addend_1, "matrix_add");
    MATRIX_DEBUG_INPTR(addend_2, "matrix_add");

    matrix_t sum = matrix_create_vals(addend_1->width, addend_1->height, addend_1->values);

    const int values = sum.width * sum.height;
    for (int i = 0; i < values; ++i)
        sum.values[i] += addend_2->values[i];

    return sum;
}

// Returns the difference of two matrices.
matrix_t matrix_sub(const matrix_t *minuend, const matrix_t *subtrahend) {
    MATRIX_DEBUG_WIDHE(minuend, subtrahend, "matrix_sub");
    MATRIX_DEBUG_INPTR(minuend, "matrix_sub");
    MATRIX_DEBUG_INPTR(subtrahend, "matrix_sub");

    matrix_t difference = matrix_create_vals(minuend->width, minuend->height, minuend->values);

    const int values = difference.width * difference.height;
    for (int i = 0; i < values; ++i)
        difference.values[i] -= subtrahend->values[i];

    return difference;
}

// Returns the product of a matrix and constant number
matrix_t matrix_mult_const(const matrix_t *multiplicand, const double multiplier) {
    MATRIX_DEBUG_INPTR(multiplicand, "matrix_mult_const");

    matrix_t product = matrix_create_vals(multiplicand->width, multiplicand->height, multiplicand->values);

    const int values = product.width * product.height;
    for (int i = 0; i < values; ++i)
        product.values[i] *= multiplier;

    return product;
}

// Returns the product of two matrices.
matrix_t matrix_mult(const matrix_t *multiplicand, const matrix_t *multiplier) {
    puts("CANNOT MULTIPLY YET");
    exit(EXIT_FAILURE);

    MATRIX_DEBUG_INPTR(multiplicand, "matrix_mult");
    MATRIX_DEBUG_INPTR(multiplier, "matrix_mult");
    MATRIX_DEBUG_MISC(multiplicand->width != multiplier->height, "UNDEFINED MULTIPLICATION IN \"matrix_mult\"!", 1);

    matrix_t product = matrix_create_uninitialized(multiplier->width, multiplicand->height);

    // TODO

    return product;
}

// For testing matrix functionality.
int main() {
    matrix_t first = matrix_create_vals(2, 2, (double[]) { 1, 2, 3, 4 });
    matrix_t second = matrix_create_vals(2, 2, (double[]) { 25, 50, 75, 100 });

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

    // Print product of difference and 5
    matrix_t constProduct = matrix_mult_const(&difference, 5.0f);
    matrix_print(&constProduct);
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
    matrix_destroy(&constProduct);
    matrix_destroy(&product);
}
