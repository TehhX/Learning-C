// Get user input without blocking until they type
// Currently Linux only

#include "stdio.h"
#include "unistd.h"
#include "stdbool.h"
#include "poll.h"
#include "signal.h"
#include "string.h"

static bool caught = false;
void sigint_handler(int _)
{
    caught = true;
}

int main()
{
    signal(SIGINT, sigint_handler);

    while (!caught)
    {
        struct pollfd poll_input =
        {
            .fd = STDIN_FILENO,
            .events = POLLPRI | POLLIN,
            .revents = 0
        };

        poll(&poll_input, 1, 0);

        if ((poll_input.revents & POLLIN) && !(poll_input.revents & POLLPRI))
        {
            char buf[256];
            fgets(buf, 256, stdin);
            printf("You entered \"%.*s\".\n", (int) (strlen(buf) - 1), buf);
        }
    }

    printf("\nCaught SIGINT, exiting.\n");

    signal(SIGINT, NULL);
}
