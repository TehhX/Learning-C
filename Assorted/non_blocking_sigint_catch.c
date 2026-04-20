// Get user input without blocking until they type. Foundational info in /Assorted/sigint_interception.c
// Linux only for the time being

#include "stdio.h"
#include "stdbool.h"
#include "string.h"

#define NBSC_PLAT_LIN 0
#define NBSC_PLAT_WIN 1

static bool caught = false;

#ifdef __linux__
    #define NBSC_PLAT NBSC_PLAT_LIN
    #define NBSC_INSERT_LIN(CONTENTS) CONTENTS
    #define NBSC_INSERT_WIN(CONTENTS)

    #include "unistd.h"
    #include "poll.h"
    #include "signal.h"
    #include "errno.h"

    void sigint_handler(int _)
    {
        caught = true;
    }
#elif defined(_WIN32)
    #define NBSC_PLAT NBSC_PLAT_WIN
    #define NBSC_INSERT_LIN(CONTENTS)
    #define NBSC_INSERT_WIN(CONTENTS) CONTENTS

    #include "windows.h"

    BOOL WINAPI sigint_handler(DWORD _)
    {
        caught = true;

        return TRUE;
    }
#else
    #error "Unknown OS."
#endif

int main()
{
    if
    (
        NBSC_INSERT_LIN(signal(SIGINT, sigint_handler) == SIG_ERR)
        NBSC_INSERT_WIN(!SetConsoleCtrlHandler(sigint_handler, TRUE))
    )
    {
        perror("Couldn't modify SIGINT handling");
        return 1;
    }

    while (!caught)
    {
        struct pollfd poll_input =
        {
            .fd = STDIN_FILENO,
            .events = POLLIN
        };

        if (poll(&poll_input, 1, 0) == -1)
        {
            // Interrupted by CTRL+C while polling, leave loop
            if (errno == EINTR)
            {
                break;
            }
            // Unknown error, exit failure
            else
            {
                perror("Poll failed with code other than EINTR");
                return 1;
            }
        }

        if (poll_input.revents & POLLIN)
        {
            char buf[256];
            fgets(buf, 256, stdin);
            printf("You entered \"%.*s\".\n", (int) (strlen(buf) - 1), buf);
        }
    }

    printf("\nCaught SIGINT, exiting.\n");

    if
    (
        NBSC_INSERT_LIN(signal(SIGINT, NULL) == SIG_ERR)
        NBSC_INSERT_WIN(!SetConsoleCtrlHandler(NULL, FALSE))
    )
    {
        perror("Couldn't restore SIGINT handling");
        return 1;
    }
}
