#include "stdlib.h"
#include "stdio.h"
#include "errno.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fputs("Provide a number as first argument.\n", stderr);
        return 1;
    }

    for (int i = 1; i < argc; ++i)
    {
        char *end;
        const long result = strtol(argv[i], &end, 10);

        if (end == argv[i] || end[0] != '\0' || errno == ERANGE)
        {
            fprintf(stderr, "Bad input: \"%s\"\n", argv[i]);
            errno = 0; // Bit sketchy, but stackoverflow says it's fine and that's all that matters :)
        }
        else
        {
            printf("Result: %ld\n", result);
        }
    }
}
