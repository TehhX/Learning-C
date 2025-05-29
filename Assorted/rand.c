#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Arg1 = count | Arg2 = min | Arg3 = max
int main(int argc, char* argv[]) {
    if (argc != 4)
        exit(1);

    const int
        count = strtol(argv[1], NULL, 10),
        min   = strtol(argv[2], NULL, 10),
        max   = strtol(argv[3], NULL, 10);

    // Set rand seed based on the current time
    srand(time(NULL));

    // Generate <count> numbers from <min> to <max> inclusive
    for (int i = 0; i < count; i++)
        printf("%d\n", rand() % (max - min + 1) + min);
}
