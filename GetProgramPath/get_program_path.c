#include "get_program_path.h"

#if defined(__linux__)
    #include "stdlib.h"
    #include "sys/param.h"
#endif

char *get_program_path(const char * const argv0)
{
#if defined(__linux__)
    return realpath(argv0, malloc(sizeof(char) * PATH_MAX));
#else
    #error UNKNOWN OPERATING SYSTEM
#endif
}
