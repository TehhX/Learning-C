#include "pthread.h"
#include "stdio.h"

void *callback(int *in)
{
    printf("%d\n", in[0]);
    in[1] = 0;
    return 0;
}

int main()
{
    pthread_t thr;

    int input[] = { 100000, 1 };
    int output, *output_p = &output;

    printf("Created: %d\n", !pthread_create(&thr, NULL, (void *(*)(void *)) callback, input));

    while (input[1])
    {
        puts("wow");
    }
}
