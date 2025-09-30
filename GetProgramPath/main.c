#include "get_program_path.h"

#include "stdio.h"
#include "stdlib.h"

int main(int argc, char **argv)
{
    char *path = get_program_path(argv[0]);

    puts(path);

    free(path);

    return 0;
}
