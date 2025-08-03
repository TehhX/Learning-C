#include "stdio.h"
#include "stdlib.h"

// Writes CHAR to FILENAME COUNT times.

int main(int argc, char **argv) {
    if (argc != 4) {
        puts("Wrong argument count. Need FILENAME CHAR COUNT");
        return EXIT_FAILURE;
    }

    char character = *argv[2];

    long count = atol(argv[3]);

    FILE *out = fopen(argv[1], "wb");
    for (long i = 0; i < count; ++i)
        fputc(character, out);

    fclose(out);

    return EXIT_SUCCESS;
}
