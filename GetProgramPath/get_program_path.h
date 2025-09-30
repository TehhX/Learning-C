#ifndef __GET_PROGRAM_PATH_H
#define __GET_PROGRAM_PATH_H

/*
Get Program Path (get_program_path()) - AIO Header File and Implementation - Returns a malloc'd string containing the absolute path to the current process (including filename and extension).

This should always return the full path to the executable, regardless of how it is executed. If the program is /home/myProgram, that's what should *always* be returned.

Tested/supported OS's:
    * Linux
    * Win64

Implementation tests:
    * In-folder: Will exectuting the program inside its return correctly, e.g ./program
    * Out-folder: Will exectuting the program outside its return correctly, e.g Folder/program
    * Shortcut: Will executing a shortcut to the program return correctly e.g programShortcut.exe
    * Soft-symlink: Will executing a soft-symlink return correctly e.g ./programLink
    * PATH Invocation: Will invoking the program from an arbitrary folder

Should any OS fail any tests they and their failed tests will be listed here:
    * N/A
*/

#include "stdlib.h" // For malloc(...).

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__windows__)
    #define __windows__ 1
#endif

#if __linux__
    #include "linux/limits.h" // For PATH_MAX
    #include "unistd.h" // For readlink(...)
#elif __windows__
    #include "libloaderapi.h" // For GetModuleFileNameA(...)
    #ifndef PATH_MAX
        #include "minwindef.h" // Contains MAX_PATH
        #define PATH_MAX MAX_PATH
    #endif // PATH_MAX
#endif

static char *get_program_path()
{
#if __linux__
    char *path = malloc(PATH_MAX);
#if defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wunused-result"
#endif // __GNUC__
    readlink("/proc/self/exe", path, PATH_MAX);
#if defined(__GNUC__)
    #pragma GCC diagnostic pop // "-Wunused-result"
#endif // __GNUC__
    return path;
#elif __windows__
    char *path = malloc(PATH_MAX);
    const int path_len = GetModuleFileNameA(NULL, path, PATH_MAX) + 1;
    path = realloc(path, path_len);

    // Replace '\' with '/' for conformity between OS's:
    for (int i = 0; path[i]; ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }

    return path;
#else
    #error Unknown operating system, currently only supports Linux and Windows-32/64.
#endif
}

#endif // __GET_PROGRAM_PATH_H
