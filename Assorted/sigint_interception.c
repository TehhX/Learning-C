// Intercepts SIGINT, runs custom action instead of default process exit. Will catch it 3 times before actually exiting

#include "stdio.h"
#include "stdbool.h"
#include "signal.h"

static bool sigint_caught = false;

void sigint_handler(int _)
{
    sigint_caught = true;
}

int main()
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
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
