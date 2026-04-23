// Enables, demonstrates windows ANSI escape sequences for coloring text
// Windows only

#include "stdio.h"
#include "stdlib.h"
#include "windows.h"

static void enable_vt100()
{
    // Get stdout handle
    const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout_handle == INVALID_HANDLE_VALUE)
    {
        printf("Couldn't get stdout_handle: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }

    // Modify current mode
    DWORD mode;

    if (!GetConsoleMode(stdout_handle, &mode))
    {
        printf("Couldn't get console mode: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }

    if (!SetConsoleMode(stdout_handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
    {
        printf("Couldn't set console mode: %d\n", GetLastError());
        exit(EXIT_FAILURE);
    }
}

int main()
{
    enable_vt100();

    puts
    (
        "\033[37mHello\n"
        "\033[36mHello\n"
        "\033[35mHello\n"
        "\033[34mHello\n"
        "\033[33mHello\n"
        "\033[32mHello\n"
        "\033[31mHello\n"
        "\033[30mHello\033[39m <= Invisible because black on black."
    );
}
