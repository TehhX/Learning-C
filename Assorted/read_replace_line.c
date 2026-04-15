#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"

// Create original file to manipulate. Not a part of the replacing program, just set-up for it
void create_original(const char *const fullname, const char *const contents)
{
    FILE *file_handle = fopen(fullname, "w");
    if (!file_handle)
    {
        fputs("Couldn't create test file.\n", stderr);
        exit(EXIT_FAILURE);
    }

    fputs(contents, file_handle);

    if (fclose(file_handle))
    {
        fputs("Failed to close test file after creation.\n", stderr);
        exit(EXIT_FAILURE);
    }
}

// Will replace line `offender` in fullname with line `defender`
void replace_line_contents(const char *const fullname, const char *const offender, const char *const defender)
{
    FILE *file_input = fopen(fullname, "r");
    if (!file_input)
    {
        fputs("Couldn't open test file for reading.\n", stderr);
        exit(EXIT_FAILURE);
    }

    // Would preferably be in temporary location
    const char temporary_fullname[] = "rrl_IGN.txt.tmp";

    FILE *file_output = fopen(temporary_fullname, "w");
    if (!file_input)
    {
        fputs("Couldn't open temp file for writing.\n", stderr);
        exit(EXIT_FAILURE);
    }

    char *buf = NULL;
    int input;

    do
    {
        size_t buf_len = 0;

        bool
            found = true,
            in_offender = true
        ;

        while ((input = fgetc(file_input)) != '\n' && input != EOF)
        {
            // Allocate space for, assign input onto buf
            buf = realloc(buf, sizeof(char) * (++buf_len));
            buf[buf_len - 1] = (char) input;

            found =
                (found) && // Has matched up to this point
                (in_offender = in_offender && (offender[buf_len - 1] != '\0')) && // Haven't gone past offender null terminator either before or just now
                (offender[buf_len - 1] == input) // Matches on this character as well
            ;
        }

        if (buf_len && found && (offender[buf_len] == '\0'))
        {
            fprintf(file_output, "%s\n", defender);
        }
        else if (buf_len)
        {
            fwrite(buf, sizeof(char), buf_len, file_output);
            fputc('\n', file_output);
        }
    }
    while (input != EOF);

    if (fclose(file_input))
    {
        fputs("Failed to close read file.\n", stderr);
        exit(EXIT_FAILURE);
    }

    if (fclose(file_output))
    {
        fputs("Failed to close write file.\n", stderr);
        exit(EXIT_FAILURE);
    }

    // Overwrite original file with temporary file to complete the process
    rename(temporary_fullname, fullname);
}

// Test lines
#define TEST_L1 "alpha"
#define TEST_L2 "beta alpha"
#define TEST_L3 "delta"
#define TEST_L4 "alpha beta"

int main()
{
    // File fullname to create and then rewrite
    const char fullname[] = "read_rep_file_IGN.txt";

    // Create the original file, not important
    create_original(fullname, TEST_L1 "\n" TEST_L2 "\n" TEST_L3 "\n" TEST_L4 "\n");

    // Replace offender with defender in file fullname
    replace_line_contents(fullname, TEST_L2, "This line was changed.");
}
