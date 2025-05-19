#include <stdio.h>

// Flag enum (combinations possible)
typedef enum {
    BOLD      = 0b001,
    UNDERLINE = 0b010,
    ITALICS   = 0b100
} FontFormats;

// Choice enum (select one)
typedef enum {
    SMALL  = 10,
    MEDIUM = 30,
    LARGE  = 80
} FontSizes;

// Combine enums
typedef struct {
    FontFormats format;
    FontSizes size;
} Font;

void printFormat(FontFormats ff) {
    if (ff & BOLD)
        printf("Bold, ");
    if (ff & UNDERLINE)
        printf("Underlined, ");
    if (ff & ITALICS)
        printf("Italisized, ");
}

void printSize(FontSizes fs) {
    switch (fs) {
    case SMALL:
        printf("Small.\n");
        break;

    case MEDIUM:
        printf("Medium.\n");
        break;

    case LARGE:
        printf("Large.\n");
    }
}

void printFont(Font* f) {
    printFormat(f->format);
    printSize(f->size);
}

int main() {
    Font titleFont    = { BOLD | UNDERLINE, LARGE };
    Font subtitleFont = { ITALICS, MEDIUM };

    printFont(&titleFont);
    printFont(&subtitleFont);
}
