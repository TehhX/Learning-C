#ifndef __GET_PROGRAM_PATH_H
#define __GET_PROGRAM_PATH_H

/*
Get Program Path (get_program_path()) - AIO Header File and Implementation - Returns a malloc'd string containing the absolute path to the current process (including filename and extension).

This should always return the full path to the executable, regardless of how it is executed. If the program is /home/myProgram, that's what should *always* be returned.

Tested/supported OS's:
    * Linux
    * Windows 10

Implementation tests:
    * In-folder: Will exectuting the program inside its return correctly, e.g ./program
    * Out-folder: Will exectuting the program outside its return correctly, e.g Folder/program
    * Shortcut: Will executing a shortcut to the program return correctly e.g programShortcut.exe
    * Soft-symlink: Will executing a soft-symlink return correctly e.g ./programLink
    * PATH invocation: Will invoking the program from an arbitrary folder return correctly e.g program

Should any OS fail any tests they and their failed tests will be listed here:
    * N/A
*/

#include "stdlib.h"

#define __windows__ (defined(_WIN32) || defined(_WIN64))

#if __linux__
    #include "linux/limits.h" // For PATH_MAX
    #include "unistd.h" // For readlink(...)
#elif __windows__
    #include "libloaderapi.h" // For GetModuleFileNameA(...)
    #ifndef PATH_MAX
        #include "minwindef.h" // Contains MAX_PATH
        #define PATH_MAX_NEW_DEFINE
        #define PATH_MAX MAX_PATH
    #endif // PATH_MAX
#endif

static char *get_program_path()
{
    char *path = malloc(PATH_MAX + 1);
    const int path_len =
#if __linux__
    readlink("/proc/self/exe", path, PATH_MAX) + 1;
#elif __windows__
    GetModuleFileNameA(0, path, PATH_MAX) + 1;

    // Replace '\' with '/' for conformity between OS's:
    for (int i = 0; i < path_len; ++i)
    {
        if (path[i] == '\\')
        {
            path[i] = '/';
        }
    }
#else
    #error Unknown operating system, currently only support Linux and Windows-32/64. Submitting new OS implementations through GitHub PRs will always be welcome.
#endif
    path[path_len - 1] = 0;
    return realloc(path, path_len);
}

// Remove new definitions:
#undef __windows__
#ifdef PATH_MAX_NEW_DEFINE
    #undef PATH_MAX
    #undef PATH_MAX_NEW_DEFINE
#endif

#endif // __GET_PROGRAM_PATH_H
