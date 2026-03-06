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
    printf("RGB( %3d, %3d, %3d )\n", c.r, c.g, c.b);
}

#define STEPS 10

int main(int argc, char **argv)
{
    // No arguments, go through steps
    if (argc == 1)
    {
        for (float i = 0; i <= 1.0; i += 1.0f / (STEPS - 1))
        {
            color_print(color_from_slider(i));
        }
    }
    // Take argument as slider value
    else
    {
        color_print(color_from_slider(atof(argv[1])));
    }
}
