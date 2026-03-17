// This program is meant to read from a file which is currently being written to by another process. Instructions taken from https://stackoverflow.com/a/44351438 and https://stackoverflow.com/a/15485689.

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#define STEP_SECONDS    (int)   1 // Seconds before rescans
#define BUFFER_BYTES (size_t) 128 // Bytes in the buffer

int main(int argc, char **argv)
{
    // 0th argument not required
    --argc; ++argv;

    // First argument should have file name
    if (argc != 1)
    {
        puts("Need file argument.");
        return 1;
    }

    // Standard file open
    FILE *const input_file = fopen(argv[0], "r");
    if (!input_file)
    {
        fprintf(stderr, "Failed to open file '%s' for reading: ", argv[0]);
        perror("");
        return 1;
    }

    do
    {
        // fgets returns NULL if no characters are returned at EOF
        for (char buffer[BUFFER_BYTES]; NULL != fgets(buffer, BUFFER_BYTES, input_file) ; )
        {
            printf(buffer);
        }

        // Clears the EOF of input_file. Not entirely sure why it's required given my modifcations, unless fgets returns NULL if EOF is set regardless of stream content
        clearerr(input_file);
    }
    // If STEP_SECONDS > 0, sleep while it returns no errors. If 0, do without sleeping forever
    while (STEP_SECONDS > 0 ? !sleep(STEP_SECONDS) : 1);
}
