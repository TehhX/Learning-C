// Check if enter key is pressed.
// Windows only, obviously

#include "windows.h"
#include "stdio.h"

int main()
{
    // Get rid of first garbage
    GetAsyncKeyState(VK_RETURN);

    for (unsigned long long count = 1; count; ++count)
    {
	const SHORT key_state = GetAsyncKeyState(VK_RETURN);
        if (key_state & 0x1)
        {
            printf("Pressed on %llu.\n", count);
        }
    }

    puts("Overflowed count variable.");
}
