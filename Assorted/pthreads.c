#include "pthread.h"
#include "stdio.h"
#include "stdint.h"
#include "inttypes.h"

// This prints val!.
void *factorial_print(const uint64_t *const restrict val)
{
    uint64_t res = 1;

    for (uint64_t i = *val; i > 1; --i)
    {
        res *= i;
    }

    printf("       Value of %" PRId64 "! is: %19" PRId64 ".\n", *val, res);

    return 0;
}

struct thread_io {
    uint64_t i;
    uint64_t *o;
};

// This progam can/should be improved. The return value is ignored as it is to difficult to work with, instead opting to have it modify an output designated pointer parameter inside a struct.
void *factorial(struct thread_io *io)
{
    *io->o = 1;

    for (uint64_t i = io->i; i > 1; --i)
    {
        *io->o *= i;
    }

    return 0;
}

/*
    Set to change multithreadedness, then time execution for each and compare. My times as an example in milliseconds:
        * gcc -O0:
            * Single: 7.7660, 7.4233, 7.4660, 7.5405, 7.2502, 7.3198 | Mean: 7.4610
            *  Multi: 7.4691, 9.3144, 8.3200, 8.8297, 8.4070, 8.5869 | Mean: 8.4879
        * gcc -O3:
            * Single: 7.7552, 7.3391, 8.2170, 9.5182, 8.2660, 8.0814 | Mean: 8.1962
            *  Multi: 8.0711, 8.9515, 8.7032, 9.4369, 8.6803, 8.2686 | Mean: 8.6852

    Even a small sample size like this shows that such a small and simple program does not benefit from multithreading. TODO: Rewrite a longer execution time program to show the difference.
*/
#define MULTITHREADED 1

#define REPETITIONS 19

#if REPETITIONS > 20

    #error Repetitions must be <= 20, as 20! is the largest factorial that can be stored in an uint64_t.

#endif

int main()
{
    // pthread with input/output.
    {
        uint64_t result;
        pthread_t thread;

        // Args: Pointer to pthread_t, callback in form (void *(*)(void *)), args to callback.
        pthread_create(&thread, 0, (void *(*)(void *)) factorial, &(struct thread_io){ .i = 20, .o = &result });
        pthread_join(thread, 0);

        printf("Single value of 20! is: %19" PRId64 ".\n", result);
    }

    {
    #if MULTITHREADED

        pthread_t threads[REPETITIONS];

    #endif

        for (int i = 0; i < REPETITIONS; ++i)
        {
        #if MULTITHREADED // Use multithreading.

            pthread_create(threads + i, 0, (void *(*)(void *)) factorial_print, &(uint64_t){ 20 - i });
            pthread_detach(threads[i]);

        #else // Don't multithread.

            factorial_print(&(uint64_t){ 20 - i });

        #endif
        }

    #if MULTITHREADED

        for (int i = 0; i < REPETITIONS; ++i)
        {
            pthread_join(threads[i], 0);
        }

    #endif
    }
}
