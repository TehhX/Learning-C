#include "get_program_path.h"

#include "stdio.h"
#include "stdlib.h"

int main()
{
    char *path = get_program_path();
    puts(path);
    free(path);
}
