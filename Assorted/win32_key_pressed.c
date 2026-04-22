// Check if enter key is pressed.
// Windows only, obviously

#include "windows.h"
#include "stdio.h"
#include "string.h"

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

    // Set up input buffer
    char *input = NULL;

    printf("Enter value: ");
    for (unsigned long long count = 1; count; ++count)
    {
        // Check that console window is in foreground, and subsequently that enter (newline) key is pressed, if so print count
        if (GetForegroundWindow() == console_window_handle && GetAsyncKeyState(VK_RETURN) & 1)
        {
            size_t len = 0;

            for (int cont = 1; cont; )
            {
                const int next = fgetc(stdin);
                switch (next)
                {
                    break; case EOF:
                    {
                        fputs("Unexpected input, shutting down.", stderr);
                        return 1;
                    }
                    break; case '\n':
                    {
                        input = realloc(input, sizeof(char) * (len + 1));
                        input[len] = '\0';
                        cont = 0;
                    }
                    break; default:
                    {
                        input = realloc(input, sizeof(char) * (len + 1));
                        input[len] = (char) next;
                        ++len;
                    }
                }
            }

            printf("Entered \"%s\" on loop #%llu\nEnter value: ", input, count);
        }
    }

    free(input);

    puts("Overflowed count variable.");
}
