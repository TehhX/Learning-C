// Check if enter key is pressed.
// Windows only, obviously

#include "windows.h"
#include "stdio.h"

int main()
{
    // Get rid of first garbage
    GetAsyncKeyState(VK_RETURN);

    // Change title of window to W32KP (<PROCESS_ID>)
    char new_title[16] = { 0 };
    sprintf(new_title, "W32KP (%d)", GetCurrentProcessId());
    SetConsoleTitleA(new_title);

    // Wait for change to take
    Sleep(5);

    // Get HWND of console window via new title
    const HWND console_window_handle = FindWindowA(NULL, new_title);

    for (unsigned long long count = 1; count; ++count)
    {
        // Check that console window is in foreground, and subsequently that enter (newline) key is pressed, if so print count
        if (GetForegroundWindow() == console_window_handle && GetAsyncKeyState(VK_RETURN))
        {
            printf("Pressed on %llu.\n", count);
        }
    }

    puts("Overflowed count variable.");
}
