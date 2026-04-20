// Capture user input in separate thread while able to do some processing in main thread

#include "pthread.h"
#include "stdio.h"
#include "errno.h"
#include "stdbool.h"

struct routine_user_input_params
{
    char input[1024];
    pthread_mutex_t input_lock;
};

void *routine_user_input(struct routine_user_input_params *const params)
{
    pthread_mutex_lock(&params->input_lock);
    fgets(params->input, sizeof(params->input), stdin);
    pthread_mutex_unlock(&params->input_lock);

    return NULL;
}

int main()
{
    struct routine_user_input_params params;
    if (pthread_mutex_init(&params.input_lock, NULL))
    {
        perror("Failed to initialize mutex");
        return 1;
    }

    int retval = -1;
    pthread_t user_input_thread;
    if (pthread_create(&user_input_thread, NULL, (void *(*)(void *)) routine_user_input, &params))
    {
        perror("Failed to spin up thread");
        retval = 1;
        goto CLEANUP_MUTEX;
    }

    for (bool cont = true; cont; )
    {
        switch (pthread_mutex_trylock(&params.input_lock))
        {
            break; case EBUSY:
            {
                cont = false;
            }
            break; case 0:
            {
                if (pthread_mutex_unlock(&params.input_lock))
                {
                    perror("Failed to unlock mutex from main");
                    retval = 1;
                    cont = false;
                }
            }
            break; default:
            {
                perror("Misc mutex error");
                retval = 1;
                goto CLEANUP_ALL;
            }
        }
    }

    for (unsigned long long loop_count = 0; ; )
    {
        switch (pthread_mutex_trylock(&params.input_lock))
        {
            // Already locked
            break; case EBUSY:
            {
                if (++loop_count == 0)
                {
                    puts("Count overflowed, continuing from 0...");
                }
            }
            // Got the lock successfully
            break; case 0:
            {
                printf("Got input after %llu loops: %s", loop_count, params.input);
                retval = 0;
                goto CLEANUP_ALL;
            }
            // Miscellaneous error
            break; default:
            {
                perror("Misc mutex error");
                retval = 1;
                goto CLEANUP_ALL;
            }
        }
    }

    CLEANUP_ALL:;
    pthread_cancel(user_input_thread);
    CLEANUP_MUTEX:;
    pthread_mutex_destroy(&params.input_lock);

    return retval;
}
