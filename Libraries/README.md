# Libraries
This folder demonstrates the creation and usage of both static and shared libraries. Keep in mind that using a library will require the library itself and library.h. Any fixes to errors, typos etc. are appreciated.

Tutorials used in the creation of this folder and README:
* [René Nyffenegger: Creating a shared and static library with [GCC]](https://renenyffenegger.ch/notes/development/languages/C-C-plus-plus/GCC/create-libraries/index)

## Table of Contents:
* [Static vs Shared](#the-difference-between-static-and-shared)
* [Creating](#creating-cmake)
* [Using](#using-cmake)

## The Difference Between Static and Shared
Static libraries came before shared (dynamically linked) libraries, but are still used. They have some important differences:
* Static libs are linked with the main program at compile time, shared libs are linked and used at runtime as required.
* Static libs are not required to be kept after compiling the final executable. On UNIX systems, shared libs must be kept in a designated library folder, or an rpath must be specified during linking which points from the exectuable to the lib. On Windows, the lib must be next to the executable or in PATH. There may also be a way to specify the location of the lib that I haven't found yet.
* Because static libraries are compiled into the final executable, they increase its size. Another side effect of this is that other programs may not use the libs without compiling them into itself as well. Shared libs may be used by multiple programs without recompiling the libs.
* Because static libs are in the executable itself and are linked at compile time, accessing their contents is faster shared libs, though most sources report it as an *incredibly* minimal overhead that shouldn't be taken into consideration in almost all cases. However, I will still note the overhead as it does exist.

## Creating (CMake)
### Static
<!-- TODO: Finish these instructions !-->
### Shared
<!-- TODO: Finish these instructions !-->
## Creating (GCC)
### Static
Compile all the library_*.c files into their own object files:

    gcc -o libA.o -c library_a.c
    gcc -o libB.o -c library_b.c
    gcc -o libC.o -c library_c.c

Use ar (archiver) to put them all into a static library (.a) file and remove intermediary files:

    ar rcs lib.a libA.o libB.o libC.o
    <rm/del> lib*.o

You will be left with a single lib.a which can now be [used](#using-cmake) in other projects.

### Shared
<!-- TODO: Finish these instructions !-->
## Using (CMake)
### Static
<!-- TODO: Finish these instructions !-->
### Shared
<!-- TODO: Finish these instructions !-->
## Using (GCC)
### Static
<!-- TODO: Finish these instructions !-->
### Shared
<!-- TODO: Finish these instructions !-->
