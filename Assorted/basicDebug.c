#include <stdio.h>

// This can also be done with GCC in the command line
// "gcc basicDebug.c -D DEBUG=<n>", where <n> is either 1 or 0
// If this is left undefined, it behaves as if it is 0
#define DEBUG 1

#if DEBUG
    #define DEBUG_LOG(LOG) printf(LOG)
    #define DEBUG_EXEC(EXEC) EXEC
#else
    #define DEBUG_LOG(LOG) ;
    #define DEBUG_EXEC(EXEC) ;
#endif

int main() {
    printf("This is in either mode.\n");

    DEBUG_LOG("This is in debug mode.\n");

    int x = 5;

    // If DEBUG is enabled, this overwrites x
    DEBUG_EXEC(
        x = 22;
    )

    printf("x is %d.\n", x);
}
