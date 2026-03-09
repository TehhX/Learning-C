#include "stdio.h"
#include "math.h"
#include "stdlib.h"

struct color
{
    int r, g, b;
};

// Derived via https://www.desmos.com/calculator/juhozs2rk3. Will take 0.0 as max red, 0.5 as max green, and 1.0 as max blue and interpolate between them
struct color color_from_slider(float slider)
{
    return (struct color)
    {
        .r = (1.0 - slider) * 255,
        .g = (0.5f - fabsf(0.5f - slider)) * 255.0f + (255.0f / 2.0f),
        .b = slider * 255
    };
}

void color_print(struct color c)
{
    printf("%3d, %3d, %3d,\n", c.r, c.g, c.b);
}

const char help_text[] =
    "| Arg Position |                          Expectation                           | Example |\n"
    "|--------------|----------------------------------------------------------------|---------|\n"
    "|      0       | Either 's' or 'v'. 's' for steps, 'v' for single slider value. |    s    |\n"
    "|   1 (0=v)    |                        Value of slider.                        |  0.25   |\n"
    "|   1 (0=s)    |                        Amount of steps                         |   100   |"
;

int main(int argc, char **argv)
{
    // Get rid of 1st argument
    --argc; ++argv;

    if (argc == 0)
    {
        puts(help_text);
        return 0;
    }
    else if (argv[0][0] == 'v')
    {
        const float value = atof(argv[1]);

        if (value > 1.0f || value < 0.0f)
        {
            puts("Value of slider must be 0.0 <= s <= 1.0.");
            return 1;
        }

        color_print(color_from_slider(value));
    }
    else if (argv[0][0] == 's')
    {
        const int steps = atol(argv[1]);

        for (float i = 0; i <= 1.0; i += 1.0f / (steps - 1))
        {
            color_print(color_from_slider(i));
        }
    }
    else
    {
        puts("Bad value at argv index 1. Review help:");
        puts(help_text);
        return 1;
    }
}
