#include "get_program_path.h"

// IMPORTANT NOTE: This program has only been tested on Ubuntu and Windows 10-64bit, one machine each. Thus, issues may arise on other systems. Report them using Git's issue system, or fix them with a PR if you feel up to it.

#include "stdlib.h" // For realpath(...), _fullpath(...).
#include "limits.h" // For PATH_MAX (Linters seem to have an issue finding PATH_MAX within, but it compiles anyway)

char *get_program_path(const char * const argv0)
{
#if defined(__linux__)
    return realpath(argv0, malloc(sizeof(char) * PATH_MAX));
#elif defined(_WIN32) || defined(_WIN64)
    // Can use GetModuleFileName in lieu of _fullpath, unsure of any difference(s).
    return _fullpath(malloc(sizeof(char) * PATH_MAX), argv0, PATH_MAX);
#else
    #error Unknown operating system, currently only supports Linux and Windows-32/64.
#endif
}
