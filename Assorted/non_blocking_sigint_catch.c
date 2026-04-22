// Get user input without blocking until they type. Foundational info in /Assorted/sigint_interception.c and /Assorted/multithreaded_user_input.c

#if !defined(__linux__) && !defined(_WIN32)
    #error "Unknown OS."
#elif !defined(__GNUC__) && !defined(_MSC_VER)
    #error "Unknown compiler."
#endif

#include "stdio.h"
#include "pthread.h"
#include "stdbool.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"

static bool caught_sigint = false;

#ifdef __GNUC__
    #define ATTR_UNUSED_PARAM __attribute__((unused))
#elif defined(_MSC_VER)
    #define ATTR_UNUSED_PARAM
#endif

#ifdef __linux__
    #include "signal.h"

    void sigint_handler(ATTR_UNUSED_PARAM const int sig_code)
    {
        caught_sigint = true;
    }
#elif defined(_WIN32)
    #include "windows.h"

    BOOL WINAPI sigint_handler(ATTR_UNUSED_PARAM const DWORD sig_code)
    {
        caught_sigint = true;

        return TRUE;
    }
#endif

struct routine_user_input_params
{
    char input[256];
    pthread_mutex_t input_lock;
};

static void *routine_user_input(struct routine_user_input_params *const params)
{
    pthread_mutex_lock(&params->input_lock);
    START:;
    if (!fgets(params->input, sizeof(params->input), stdin))
    {
        goto START;
    }
    pthread_mutex_unlock(&params->input_lock);

    return NULL;
}

// Exits program with value `retval` after cleaning thread and mutex. Pass NULL to `thread` and/or `mutex` for no cleanup on either respectively. Pass `true` to `sigint_status` to reset signal catching behavior of terminal
static void cleanup(int retval, pthread_t *const thread, pthread_mutex_t *const mutex, const bool sigint_status)
{
    if (thread)
    {
        if ((errno = pthread_cancel(*thread))
        // Windows complains without this
        #ifdef _WIN32
            && errno != ESRCH
        #endif
        )
        {
            perror("Cleanup: Couldn't cancel user input thread");
            retval = 1;
        }
    }

    if (mutex)
    {
        if ((errno = pthread_mutex_unlock(mutex)))
        {
            perror("Cleanup: Couldn't unlock mutex");
            retval = 1;
        }

        if ((errno = pthread_mutex_destroy(mutex)))
        {
            perror("Cleanup: Couldn't destroy mutex");
            retval = 1;
        }
    }

    if (sigint_status)
    {
        if (
        #ifdef __linux__
            signal(SIGINT, NULL) == SIG_ERR
        #elif defined(_WIN32)
            !SetConsoleCtrlHandler(NULL, FALSE)
        #endif
        ){
            perror("Cleanup: Couldn't restore SIGINT handling");
            retval = 1;
        }
    }

    exit(retval);
}

int main()
{
    if (
    #ifdef __linux__
        signal(SIGINT, sigint_handler) == SIG_ERR
    #elif defined(_WIN32)
        !SetConsoleCtrlHandler(sigint_handler, TRUE)
    #endif
    ){
        perror("Couldn't modify SIGINT handling");
        cleanup(1, NULL, NULL, false);
    }

    struct routine_user_input_params params;
    if (pthread_mutex_init(&params.input_lock, NULL))
    {
        perror("Failed to initialize mutex");
        cleanup(1, NULL, NULL, true);
    }

    pthread_t user_input_thread;
    if (pthread_create(&user_input_thread, NULL, (void *(*)(void *)) routine_user_input, &params))
    {
        perror("Failed to spin up thread");
        cleanup(1, NULL, &params.input_lock, true);
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
                    cleanup(1, &user_input_thread, &params.input_lock, true);
                }
            }
            break; default:
            {
                perror("Misc mutex error");
                cleanup(1, &user_input_thread, &params.input_lock, true);
            }
        }
    }

    printf("Enter string: ");

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

                if (caught_sigint)
                {
                    // Separate ^C output and SIGINT caught, only required on Linux
                    #ifdef __linux__
                        printf(" | ");
                        fflush(stdout);
                    #endif

                    printf("SIGINT caught.\nEnter string: ");
                    fflush(stdout);
                    caught_sigint = false;
                }
            }
            // Got the lock successfully
            break; case 0:
            {
                printf("Got input after %llu loops: \"%.*s\"", loop_count, (int) strlen(params.input) - 1, params.input);

                // Linux doesn't insert '\n' after exiting, windows does
                #ifdef __linux__
                    fputc('\n', stdout);
                #endif

                cleanup(0, &user_input_thread, &params.input_lock, true);
            }
            // Miscellaneous error
            break; default:
            {
                perror("Misc mutex error");
                cleanup(1, &user_input_thread, &params.input_lock, true);
            }
        }
    }

    cleanup(1, &user_input_thread, &params.input_lock, true);
}
