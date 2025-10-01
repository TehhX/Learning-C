#ifndef __GET_PROGRAM_PATH_H
#define __GET_PROGRAM_PATH_H

/*
Get Program Path (get_program_path()) - AIO Header File and Implementation - Returns a malloc'd string containing the absolute path to the current process (including filename and extension).

This should always return the full path to the executable, regardless of how it is executed. If the program is /home/myProgram, that's what should *always* be returned.

Define GPP_DELIM as one of the following:
    * 'P': Add a new parameter to get_program_path(...) to accept a delimiter per-call
    * 'D': Use the default delimiter for the OS (Will not always be the same across OSes). The default if GPP_DELIM isn't defined.
    * '\': Separate directories with a backslash (\)
    * '/': Separate directories with a slash (/)

Tested/supported OS's:
    * Linux
    * Windows 10

Implementation tests:
    * In-folder: Will exectuting the program inside its return correctly, e.g ./program
    * Out-folder: Will exectuting the program outside its return correctly, e.g Folder/program
    * Shortcut: Will executing a shortcut to the program return correctly e.g programShortcut.exe
    * Soft-symlink: Will executing a soft-symlink return correctly e.g ./programLink
    * PATH invocation: Will invoking the program from an arbitrary folder return correctly e.g program
*/

#include "stdlib.h"

#define __windows__ (defined(_WIN32) || defined(_WIN64))

#if __linux__
    #include "linux/limits.h" // For PATH_MAX
    #include "unistd.h" // For readlink(...)
    #define __GPP_DEFAULT_DELIM '/'
#elif __windows__
    #include "libloaderapi.h" // For GetModuleFileNameA(...)
    #define __GPP_DEFAULT_DELIM '\\'
    #ifndef PATH_MAX
        #include "minwindef.h" // Contains MAX_PATH
    #endif // PATH_MAX
#endif

// If GPP_DELIM not specified, use system default:
#ifndef GPP_DELIM
    #define GPP_DELIM 'D'
#endif

#define __BASE_RPL_DELIM(NEW) { for(int i=0;i<path_len;++i)if(path[i]==__GPP_DEFAULT_DELIM)path[i]=NEW; }

#if ('D' == GPP_DELIM) || (__GPP_DEFAULT_DELIM == GPP_DELIM)
    #define __GPP_PARAM
    #define __RPL_DELIM()
#elif ('P' == GPP_DELIM)
    #define __GPP_PARAM const char delim
    #define __RPL_DELIM() __BASE_RPL_DELIM(delim)
#elif (__GPP_DEFAULT_DELIM != GPP_DELIM) && (GPP_DELIM == '\\' || GPP_DELIM == '/')
    #define __GPP_PARAM
    #define __RPL_DELIM() __BASE_RPL_DELIM(GPP_DELIM)
#else
    #error BAD GPP_DELIM
#endif

static char *get_program_path(__GPP_PARAM)
{
    char *path = malloc(PATH_MAX + 1);
    const int path_len =
#if __linux__
    readlink("/proc/self/exe", path, PATH_MAX) + 1;
#elif __windows__
    GetModuleFileNameA(0, path, PATH_MAX) + 1;
#else
    #error Unknown operating system, currently only support Linux and Windows-32/64. Submitting new OS implementations through GitHub PRs will always be welcome.
#endif
    __RPL_DELIM();
    path[path_len - 1] = 0;
    return realloc(path, path_len);
}

// Remove new definitions:
#undef __windows__
#undef __GPP_PARAM
#undef __GPP_DEFAULT_DELIM
#undef __BASE_RPL_DELIM
#undef __RPL_DELIM

#endif // __GET_PROGRAM_PATH_H
