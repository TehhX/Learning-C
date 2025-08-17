#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// The decimal precision of matrix printed doubles. Define when compiling to override the default value of 2 places.
#ifndef MATRIX_PRINT_DEC_PLACES
#define MATRIX_PRINT_DEC_PLACES "2"
#endif // MATRIX_PRINT_DEC_PLACES

// May take longer to run a given program, but will run checks on all matrix operations if defined during compilation.
// MATRIX_DEBUG_INLINE will make a function static inline only when debug macro is NOT defined. For when a function is usually small, but debug statements would make it larger.
#ifdef MATRIX_DEBUG
    #define MATRIX_DEBUG_MISC(COND, WARN, EXITS) if (COND) { puts(WARN); if (EXITS) abort(); }
    #define MATRIX_DEBUG_INPTR(PTR, NAME) if (!PTR) { puts("NULL MATRIX POINTER IN \"" NAME "\"!"); abort(); } else if (!PTR->values) { puts("NULL MATRIX POINTER VALUES IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_WIDHE(INPTR_1, INPTR_2, NAME) if (INPTR_1->width != INPTR_2->width || INPTR_1->height != INPTR_2->height) { puts("WIDTH/HEIGHT MISMATCH IN \"" NAME "\"!"); abort(); }
    #define MATRIX_DEBUG_INLINE
#else // !MATRIX_DEBUG
    #define MATRIX_DEBUG_MISC(X, Y, Z) ;
    #define MATRIX_DEBUG_INPTR(X, Y) ;
    #define MATRIX_DEBUG_WIDHE(X, Y, Z) ;
    #define MATRIX_DEBUG_INLINE static inline
#endif // MATRIX_DEBUG

// Contains values, width/height of a mathematical matrix.
typedef struct matrix_t {
    int width;
    int height;
    double * values;
} matrix_t;

// Creates a matrix without ininitalizing zeros or values, malloc style.
matrix_t *matrix_create_uninitialized(const int width, const int height) {
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD WIDTH/HEIGHT IN \"matrix_create_uninitialized\"!", 1);

    matrix_t *matrix = (matrix_t*) malloc(sizeof(matrix_t));
    *matrix = (matrix_t) { .width = width, .height = height };
    matrix->values = malloc(sizeof(double) * width * height);

    return matrix;
}

// Creates a matrix with all zeros initialized, calloc style.
matrix_t *matrix_create_initialized(const int width, const int height) {
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD WIDTH/HEIGHT IN \"matrix_create_initialized\"!", 1);

    matrix_t *matrix = (matrix_t*) malloc(sizeof(matrix_t));
    *matrix = (matrix_t) { .width = width, .height = height };
    matrix->values = calloc(width * height, sizeof(double));

    return matrix;
}

// Returns a matrix from width, height, and values.
matrix_t *matrix_create_vals(const int width, const int height, const double values[]) {
    MATRIX_DEBUG_MISC(!values, "BAD VALUES IN \"matrix_create_vals\"!", 1);
    MATRIX_DEBUG_MISC(width < 1 || height < 1, "BAD MATRIX WIDTH/HEIGHT IN \"matrix_create_vals\"!", 1);

    matrix_t *matrix = matrix_create_uninitialized(width, height);
    memcpy(matrix->values, values, sizeof(double) * width * height);

    return matrix;
}

// Frees the memory used by a matrix.
MATRIX_DEBUG_INLINE void matrix_destroy(matrix_t *m) {
    MATRIX_DEBUG_INPTR(m, "matrix_destroy");

    free(m->values);
    free(m);
}

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

// Returns augend plus addend.
matrix_t *matrix_add(const matrix_t *augend, const matrix_t *addend) {
    MATRIX_DEBUG_WIDHE(augend, addend, "matrix_add");
    MATRIX_DEBUG_INPTR(augend, "matrix_add");
    MATRIX_DEBUG_INPTR(addend, "matrix_add");

    matrix_t *sum = matrix_create_vals(augend->width, augend->height, augend->values);

    const int values = sum->width * sum->height;
    for (int i = 0; i < values; ++i)
        sum->values[i] += sum->values[i];

    return sum;
}

// Returns minuend minus subtrahend.
matrix_t *matrix_sub(const matrix_t *minuend, const matrix_t *subtrahend) {
    MATRIX_DEBUG_WIDHE(minuend, subtrahend, "matrix_sub");
    MATRIX_DEBUG_INPTR(minuend, "matrix_sub");
    MATRIX_DEBUG_INPTR(subtrahend, "matrix_sub");

    matrix_t *difference = matrix_create_vals(minuend->width, minuend->height, minuend->values);

    const int values = difference->width * difference->height;
    for (int i = 0; i < values; ++i)
        difference->values[i] -= subtrahend->values[i];

    return difference;
}

// Returns multiplicand multiplied by multiplier.
matrix_t *matrix_mult_const(const matrix_t *multiplicand, const double multiplier) {
    MATRIX_DEBUG_INPTR(multiplicand, "matrix_mult_const");

    matrix_t *product = matrix_create_vals(multiplicand->width, multiplicand->height, multiplicand->values);

    if (multiplier == 1)
        return product;

    const int values = product->width * product->height;
    for (int i = 0; i < values; ++i)
        product->values[i] *= multiplier;

    return product;
}

// Returns the product of two matrices.
matrix_t *matrix_mult(const matrix_t *multiplicand, const matrix_t *multiplier) {
    MATRIX_DEBUG_INPTR(multiplicand, "matrix_mult");
    MATRIX_DEBUG_INPTR(multiplier, "matrix_mult");
    MATRIX_DEBUG_MISC(multiplicand->width != multiplier->height, "UNDEFINED MULTIPLICATION IN \"matrix_mult\"!", 1);

    matrix_t *product = matrix_create_initialized(multiplier->width, multiplicand->height);

    for (int finalY = 0; finalY < product->height; ++finalY)
        for (int finalX = 0; finalX < product->width; ++finalX)
            for (int takeOffset = 0; takeOffset < multiplicand->width; ++takeOffset)
                *matrix_get_mut(product, finalX, finalY) += matrix_get(multiplicand, takeOffset, finalY) * matrix_get(multiplier, finalX, takeOffset);

    return product;
}

// Divides dividend by divisor, returns dividend.
matrix_t *matrix_div_const(matrix_t *dividend, const double divisor) {
    MATRIX_DEBUG_INPTR(dividend, "matrix_div_const");
    MATRIX_DEBUG_MISC(!divisor, "DIV BY ZERO IN \"matrix_div_const\"!", 1);

    matrix_t *quotient = matrix_create_vals(dividend->width, dividend->height, dividend->values);

    const int values = quotient->width * quotient->height;
    for (int i = 0; i < values; ++i)
        quotient->values[i] /= divisor;

    return quotient;
}

// Returns the quotient of two matrices.
matrix_t *matrix_div(const matrix_t *dividend, const matrix_t *divisor) {
    MATRIX_DEBUG_INPTR(dividend, "matrix_div");
    MATRIX_DEBUG_INPTR(divisor, "matrix_div");
    MATRIX_DEBUG_MISC(dividend->width != divisor->height || divisor->width != divisor->height, "UNDEFINED DIVISION IN \"matrix_div\"!", 1);

    matrix_t *quotient = matrix_create_initialized(1, 1);

    // TODO

    return quotient;
}

int main() {
    matrix_t *first = matrix_create_vals(2, 2, (double[]) { 1, 2, 3, 4 });
    matrix_print(first);
    putc('\n', stdout);

    matrix_t *second = matrix_create_vals(2, 2, (double[]) { 25, 50, 75, 100 });
    matrix_print(second);
    putc('\n', stdout);

    matrix_t *third = matrix_create_vals(2, 2, (double[]) { 2.1, 3.14, 9.81, 1.7 });
    matrix_print(third);
    putc('\n', stdout);

    matrix_t *fourth = matrix_create_vals(2, 3, (double[]) { 4.5, 2.3, 5.5, 3.2, 10.5, 9.2 });
    matrix_print(fourth);
    putc('\n', stdout);

    matrix_t *sum = matrix_add(first, second);
    matrix_print(sum);
    putc('\n', stdout);

    matrix_t *sum_2 = matrix_add(sum, third);
    matrix_destroy(sum);
    matrix_print(sum_2);
    matrix_destroy(sum_2);
    putc('\n', stdout);

    matrix_t *difference = matrix_sub(first, second);
    matrix_print(difference);
    putc('\n', stdout);

    matrix_t *constProduct = matrix_mult_const(difference, 5.3);
    matrix_print(constProduct);
    matrix_destroy(constProduct);
    putc('\n', stdout);

    matrix_t *product = matrix_mult(fourth, third);
    matrix_destroy(third);
    matrix_destroy(fourth);
    matrix_print(product);
    matrix_destroy(product);
    putc('\n', stdout);

    matrix_t *constQuotient = matrix_div_const(second, 3.2);
    matrix_destroy(second);
    matrix_print(constQuotient);
    matrix_destroy(constQuotient);
    putc('\n', stdout);

    matrix_t *quotient = matrix_div(difference, first);
    matrix_destroy(difference);
    matrix_destroy(first);
    matrix_print(quotient);
    matrix_destroy(quotient);
}
