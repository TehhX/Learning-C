#include <stdio.h>
#include <stdbool.h>

int main() {
    // Standard var
    int a = 5;
    a *= 2;

    // Const var
    const float b = 3.14;
    // b = 2.3; Error, const

    // Bases (Same syntax as C++)
    int c = 0xFF; // FF hex = 255 dec
    int d = 0b11; // 11 bin = 003 dec

    // Chars
    char e = 'A';
    char f = 65;   // Same as 'A'
    char g = '\n'; // Newline char is just int(10)

    // Bools (Require stdbool.h) (Same syntax as C++)
    bool h = true;
    bool i = false;
    bool j = h && i; // AND
    bool k = h || i; // OR

    // Strings
    char l[] = "Timmy";
    char m[] = { 'A', 'B', 'C' };

    // Modifiers
    signed int n   = -24;
    unsigned int o = +15;
}
