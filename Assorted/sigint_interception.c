// Intercepts SIGINT, runs custom action instead of default process exit. Will catch it 3 times before actually exiting

#define SI_PLAT_LIN 0
#define SI_PLAT_WIN 1

#ifdef __linux__
    #define SI_PLAT SI_PLAT_LIN
    #define SI_INSERT_LIN(CONTENTS) CONTENTS
    #define SI_INSERT_WIN(CONTENTS)
#elif defined(_WIN32)
    #define SI_PLAT SI_PLAT_WIN
    #define SI_INSERT_LIN(CONTENTS)
    #define SI_INSERT_WIN(CONTENTS) CONTENTS
#else
    #error "Unknown OS."
#endif

#include "stdio.h"
#include "stdbool.h"

static bool sigint_caught = false;

#if SI_PLAT == SI_PLAT_LIN
    #include "signal.h"

    void sigint_handler(int _)
    {
        sigint_caught = true;
    }
#elif SI_PLAT == SI_PLAT_WIN
    #include "windows.h"

    BOOL WINAPI sigint_handler(DWORD _)
    {
        sigint_caught = true;

        return TRUE;
    }
#endif

int main()
{
    // Set up catching
    if
    (
        SI_INSERT_LIN(signal(SIGINT, sigint_handler) == SIG_ERR)
        SI_INSERT_WIN(!SetConsoleCtrlHandler(sigint_handler, TRUE))
    )
    // Will execute block if failed
    {
        perror("Couldn't modify SIGINT handling");
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

    // Reset terminal SIGINT catching
    if
    (
        SI_INSERT_LIN(signal(SIGINT, NULL) == SIG_ERR)
        SI_INSERT_WIN(!SetConsoleCtrlHandler(NULL, FALSE))
    )
    {
        perror("Couldn't restore SIGINT handling");
        return 1;
    }
}
