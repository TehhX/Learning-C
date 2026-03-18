#include "stdio.h"
#include "time.h"

static long since_epoch_days()
{
    // Returns (seconds since epoch) / (seconds in a day)
    return time(NULL) / (24 * 60 * 60);
}

int main()
{
    printf("Days since epoch: %ld\n", since_epoch_days());
}
