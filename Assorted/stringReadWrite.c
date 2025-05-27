#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_ONE 3
#define SIZE_TWO 26

#define VALUE_ONE "xyz"
#define VALUE_TWO "abcdefghijklmnopqrstuvwxyz"

void write() {
    FILE* out = fopen("myStringFile.txt", "w");

    if (out == NULL) {
        printf("Could not open writing file.\n");
        exit(1);
    }

    char* valOne = VALUE_ONE;
    fwrite(valOne, 1, SIZE_ONE, out);

    fwrite("\n", 1, 1, out);

    char* valTwo = VALUE_TWO;
    fwrite(valTwo, 1, SIZE_TWO, out);

    if (fclose(out)) {
        printf("Could not close writing file.\n");
        exit(1);
    }
}

void read() {
    FILE* in = fopen("myStringFile.txt", "r");

    if (in == NULL) {
        printf("Could not open reading file.\n");
        exit(1);
    }

    char valOne[SIZE_ONE + 1];
    valOne[SIZE_ONE] = '\0';
    fread(valOne, 1, SIZE_ONE, in);
    printf("valOne: %s\n", valOne);
    if (strcmp(valOne, VALUE_ONE))
        printf("Value one does not match.\n");

    {   // Newline character
        char newline[1];
        fread(newline, 1, 1, in);
    }

    char valTwo[SIZE_TWO + 1];
    valTwo[SIZE_TWO] = '\0';
    fread(valTwo, 1, SIZE_TWO, in);
    printf("valTwo: %s\n", valTwo);
    if (strcmp(valTwo, VALUE_TWO))
        printf("Value two does not match.\n");

    if (fclose(in)) {
        printf("Could not close reading file.\n");
        exit(1);
    }
}

int main() {
    write();
    read();
}
