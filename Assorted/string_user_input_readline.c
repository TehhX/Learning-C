#include "stdio.h"
#include "stdlib.h"

// Requires GNU readline library and headers
#include "readline/readline.h"

int main()
{
    char *input = readline("Enter string: ");
    printf("You entered \"%s\"\n", input);
    free(input);
}
