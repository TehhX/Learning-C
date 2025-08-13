#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MATRICES_DEBUG

typedef struct matrix_t {
    int width;
    int height;
    int *values;
} matrix_t;

static inline int matrix_get(const matrix_t *m, int x, int y) {
#ifdef MATRICES_DEBUG
    if (!m)
        puts("NULL MATRIX POINTER \"matrix_get\"!");

    if (x < 0 || y < 0)
        puts("BAD X/Y \"matrix_get\"!");
#endif
    return m->values[x + y * m->width];
}

matrix_t matrix_create(int w, int h, int v[]) {
#ifdef MATRICES_DEBUG
    if (!v)
        puts("BAD VALUES \"matrix_create\"!");

    if (w < 1 || h < 1)
        puts("BAD WIDTH_HEIGHT \"matrix_create\"!");
#endif
    matrix_t m = { .width = w, .height = h };
    m.values = malloc(sizeof(int) * w * h);
    memcpy(m.values, v, sizeof(int) * w * h);

    return m;
}

static inline void matrix_destroy(matrix_t *m) {
#ifdef MATRICES_DEBUG
    if (!m)
        puts("NULL MATRIX POINTER \"matrix_destroy\"!");

    if (!m->values)
        puts("UNINITIALIZED MATRIX VALUES POINTER \"matrix_destroy\"!");
#endif
    free(m->values);
}

void matrix_fprint(FILE *stream, const matrix_t *m) {
#ifdef MATRICES_DEBUG
    if (!m)
        puts("NULL MATRIX POINTER \"matrix_print\"!");

    if (!m->values)
        puts("UNINITIALIZED MATRIX VALUES POINTER \"matrix_fprint\"!");

    if (!stream)
        puts("NULL FILE POINTER IN \"matrix_fprint\"!");
#endif
    for (int y = 0; y < m->height; ++y) {
        for (int x = 0; x < m->width; ++x)
            fprintf(stream, "%d ", matrix_get(m, x, y));
        putc('\n', stream);
    }
}

static inline void matrix_print(const matrix_t *m) {
    matrix_fprint(stdout, m);
}

matrix_t matrix_add(const matrix_t *a, const matrix_t *b) {
#ifdef MATRICES_DEBUG
    if (a->width != b->width || a->height != b->height)
        puts("WIDTH OR HEIGHT MISMATCH IN \"matrix_add\"!");

    if (!a || !b)
        puts("NULL MATRIX POINTER \"matrix_add\" INPUT!");

    if (!a->values || !b->values)
        puts("UNINITIALIZED MATRIX VALUES POINTER \"matrix_add\" INPUT!");
#endif
    matrix_t ret = { .width = a->width, .height = a->height };

    const int valuesSize = sizeof(int) * ret.width * ret.height;
    ret.values = malloc(valuesSize);
    memcpy(ret.values, a->values, valuesSize);

    for (int i = 0; i < valuesSize / sizeof(int); ++i)
        ret.values[i] += b->values[i];

    return ret;
}

matrix_t matrix_sub(const matrix_t *minuend, const matrix_t *subtrahend) {
#ifdef MATRICES_DEBUG
    if (!minuend || !subtrahend)
        puts("NULL MATRIX POINTER \"matrix_sub\" INPUT!");

    if (!minuend->values || !subtrahend->values)
        puts("UNINITIALIZED MATRIX VALUES POINTER \"matrix_sub\" INPUT!");

    if (minuend->width != subtrahend->width || minuend->height != subtrahend->height)
        puts("WIDTH OR HEIGHT MISMATCH IN \"matrix_sub\"!");
#endif
    matrix_t ret = { .width = minuend->width, .height = minuend->height };

    const int valuesSize = sizeof(int) * ret.width * ret.height;
    ret.values = malloc(valuesSize);
    memcpy(ret.values, minuend->values, valuesSize);

    for (int i = 0; i < valuesSize / sizeof(int); ++i)
        ret.values[i] -= subtrahend->values[i];

    return ret;
}

// TODO: Multiplication, division etc.

int main() {
    matrix_t first = matrix_create(2, 2, (int[]){ 1, 2, 3, 4 });
    matrix_t secon = matrix_create(2, 2, (int[]){ 25, 50, 75, 100 });

    // Print first
    matrix_print(&first);
    putc('\n', stdout);

    // Print secon
    matrix_print(&secon);
    putc('\n', stdout);

    // Print sum of first and secon
    matrix_t sum = matrix_add(&first, &secon);
    matrix_print(&sum);
    putc('\n', stdout);

    // Print difference of first and secon
    matrix_t difference = matrix_sub(&first, &secon);
    matrix_print(&difference);
    putc('\n', stdout);

    // Clean up
    matrix_destroy(&first);
    matrix_destroy(&secon);
    matrix_destroy(&sum);
    matrix_destroy(&difference);
}
