// This program is meant to read from a file which is currently being written to by another process. Instructions taken from https://stackoverflow.com/a/44351438.

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define STEP_SECONDS 5

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        puts("Need file argument.");
        return 1;
    }

    FILE *const input_file = fopen(argv[1], "r");
    if (!input_file)
    {
        fprintf(stderr, "Failed to open file '%s' for reading: ", argv[1]);
        perror("");
        return 1;
    }

    do
    {
        // TODO: Buffer characters more than 1 at a time
        for (int c; (c = getc(input_file)) != EOF;)
        {
            putchar(c);
        }

        clearerr(input_file);
    }
    while (!sleep(STEP_SECONDS));
}
