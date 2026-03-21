#ifndef __linux__
    // TODO: Try GetFullPathName for Windows
    #error This is a Linux specific example.
#endif

#include "stdio.h"
#include "limits.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fputs("Need first argument as path to realize.\n", stderr);
        return 1;
    }

    char *realized_path = realpath(argv[1], NULL);

    if (!realized_path)
    {
        fprintf(stderr, "Bad input: \"%s\".\n", argv[1]);
        return 1;
    }

    printf("Input \"%s\" -> \"%s\"\n", argv[1], realized_path);

    free(realized_path);
}
