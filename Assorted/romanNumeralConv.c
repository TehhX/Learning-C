#include <stdlib.h>
#include <stdio.h>

int convertNumeral(char chr) {
    switch (chr) {
    default:            return    0;
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

int main() {
    char userInput[64];

    printf("Enter numerals: ");
    scanf("%s", userInput);

    size_t sizeOfVals = 0;
    int* values = malloc(0);

    for (size_t i = 0; userInput[i] != '\0'; i++) {
        values = realloc(values, ++sizeOfVals * sizeof(int));

        if ((values[i] = convertNumeral(userInput[i])) == 0) {
            printf("Bad value entered.\n");
            exit(1);
        }
    }

    int total = 0, buffer = 0;

    for (size_t i = 0; i < sizeOfVals - 1; i++)
        calculateRelNumeral(&total, &buffer, values[i], values[i + 1]);

    calculateAbsNumeral(&total, &buffer, values[sizeOfVals - 1]);

    printf("Total: %d\n", total);

    free(values);
    return 0;
}
