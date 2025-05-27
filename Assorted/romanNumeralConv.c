#include <stdlib.h>
#include <stdio.h>

int convertNumeral(char chr) {
    switch (chr) {
    default:            return   -1;
    case 'I': case 'i': return    1;
    case 'V': case 'v': return    5;
    case 'X': case 'x': return   10;
    case 'L': case 'l': return   50;
    case 'C': case 'c': return  100;
    case 'D': case 'd': return  500;
    case 'M': case 'm': return 1000;
    }
}

void calculateRelNumeral(int* total, int* buffer, int current, int next) {
    if (current < next) {
        *total -= *buffer + current;
        *buffer = 0;
    }

    else if (current > next) {
        *total += *buffer + current;
        *buffer = 0;
    }

    else // current == next
        *buffer += current;
}

inline static void calculateAbsNumeral(int* total, int* buffer, int current) {
    return calculateRelNumeral(total, buffer, current, 0);
}

#define MAX_CHARS 32

int main() {
    char userInput[MAX_CHARS];

    printf("Enter numerals: ");
    scanf("%s", userInput);

    size_t sizeOfVals = 0;
    int values[MAX_CHARS];

// sizeOfVals acts as the index for this loop. Defining 'i' for readability
#define i sizeOfVals
    while (userInput[i] != '\0') {
        int current = convertNumeral(userInput[i]);

        if (current == -1) {
            printf("Bad value entered.\n");
            exit(1);
        }
        else
            values[i++] = current;
    }
#undef i

    int total = 0, buffer = 0;

    for (size_t i = 0; i < sizeOfVals - 1; i++)
        calculateRelNumeral(&total, &buffer, values[i], values[i + 1]);

    calculateAbsNumeral(&total, &buffer, values[sizeOfVals - 1]);

    printf("Total: %d\n", total);

    return 0;
}
