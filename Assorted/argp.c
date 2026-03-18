// Tutorial followed from https://download-mirror.savannah.gnu.org/releases/argpbook/step-by-step-into-argp.pdf

#include "stdio.h"
#include "stdlib.h"
#include "argp.h"

static int parse_options(int key, __attribute__((unused)) char *arg, __attribute__((unused)) struct argp_state *state)
{
    switch (key)
    {
    case 'd':
        if (arg)
        {
            const int total = atoi(arg);
            for (int i = 0; i < total; ++i)
            {
                putchar('.');
            }

            putchar('\n');
        }
        else
        {
            puts(".");
        }

        break;

    case 256:
        return parse_options('d', "3", state);

    case 257:
        argp_error(state, "You failed the program.\n");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    struct argp argp =
    {
        (struct argp_option [])
        {
            {
                .name = "draw-dot", // The long name for this option. Invoke via --draw-dot
                .key = 'd', // The short name for this option. Invoke via -d
                .arg = "Count", // What should be supplied after this option
                .flags = OPTION_ARG_OPTIONAL, // Optional flags
                .doc = "Show a dot on the screen.", // Doc string to display during --help
                .group = 0 // Group number for grouping on the --help screen
            },
            {
                .name = "period",
                .key = 0,
                .flags = OPTION_ALIAS // An alias for the closest previous non-alias argument option, in this case draw-dot
            },
            {
                .name = "ellipsis",
                .key = 256, // Not a printable character, thus not set as a short option. This value will still be passed to parse_options(...) for differentiating options, however
                .doc = "Show an ellipsis."
            },
            {
                .name = "fail",
                .key = 257,
                .doc = "Fails the process."
            },
            {
                .name = 0, // The last entry must have a name and key of 0, similar to the NULL terminator in a string
                .key  = 0
            }
        },
        .parser = parse_options, // The function to parse the options
        .args_doc = "There's text up here.\nMaybe another way to use it.", // Text to go in help up top,
        .doc = "There's also text down here.",
        .children = NULL, // Children options
        .argp_domain = NULL // Unsure, not useful though
    };

    if (!argp_parse(&argp, argc, argv, 0, 0, 0))
    {
        puts("\nNo errors.");
        return 0;
    }
    else
    {
        perror("Encountered an error while parsing.");
        return 1;
    }
}
