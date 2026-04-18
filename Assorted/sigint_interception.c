// Intercepts SIGINT, runs custom action instead of default process exit. Will catch it 3 times before actually exiting

#include "stdio.h"
#include "stdbool.h"

static bool sigint_caught = false;

#ifdef __linux__
    #include "signal.h"

    void sigint_handler(int _)
    {
        sigint_caught = true;
    }
#elif defined(_WIN32) || defined(_WIN64)
    #include "windows.h"

    BOOL sigint_handler(DWORD _)
    {
        sigint_caught = true;

        return TRUE;
    }
#else
    #error "Unknown OS."
#endif

int main()
{
#ifdef __linux__
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
#elif defined(_WIN32) || defined(_WIN64)
    if (!SetConsoleCtrlHandler(sigint_handler, TRUE))
#else
    #error "Unknown OS."
#endif
    {
        perror("Couldn't change signal");
        return 1;
    }

    for (int times_caught = 0; times_caught < 3; )
    {
        if (sigint_caught)
        {
            printf("Caught SIGINT %d times.\n", ++times_caught);
            sigint_caught = false;
        }
    }
}
