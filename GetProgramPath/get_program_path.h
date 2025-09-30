#ifndef __GET_PROGRAM_PATH_H
#define __GET_PROGRAM_PATH_H

// IMPORTANT NOTE: This program has only been tested on Ubuntu and Windows 10-64bit, one machine each. Thus, issues may arise on other systems. Report them using Git's issue system, or fix them with a PR if you feel up to it.

#include "stdlib.h" // For _fullpath(...), malloc(...).

#if defined(__linux__)
    #include "linux/limits.h" // For PATH_MAX
    #include "unistd.h" // For readlink(...)

#elif defined(_WIN32) || defined(_WIN64)
    // TODO: Include PATH_MAX for Windows.

#endif

static char *get_program_path(const char * const argv0)
{
#if defined(__linux__)
// Implementation works for: In-folder, out-folder, soft-symlink, PATH invocation
    char *path = malloc(PATH_MAX);
#if defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wunused-result"
#endif // __GNUC__
    readlink("/proc/self/exe", path, PATH_MAX);
#if defined(__GNUC__)
    #pragma GCC diagnostic pop // "-Wunused-result"
#endif // __GNUC__
    return path;

#elif defined(_WIN32) || defined(_WIN64)
// Implementation works for: UNTESTED
    // Can use GetModuleFileName in lieu of _fullpath, unsure of any difference(s).
    return _fullpath(malloc(sizeof(char) * PATH_MAX), argv0, PATH_MAX);

#else
    #error Unknown operating system, currently only supports Linux and Windows-32/64.

#endif
}

#endif // __GET_PROGRAM_PATH_H
