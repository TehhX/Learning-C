#include "library.h"

// This file must be linked to the relevant library output when compiled. Read ./README.md for more information.

#include "stdio.h"

int main()
{
    library_print_info();

    printf("%d\n", library_ret_val(10));

    library_exit();
}
