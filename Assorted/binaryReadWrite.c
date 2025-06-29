#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_ONE unsigned char
#define TYPE_TWO int
#define TYPE_THREE unsigned long long
#define TYPE_FOUR signed char
#define TYPE_FIVE float

#define F_SPEC_ONE "%d"
#define F_SPEC_TWO "%d"
#define F_SPEC_THREE "%llu"
#define F_SPEC_FOUR "%s"
#define F_SPEC_FIVE "%f"

#define SIZE_ONE 1
#define SIZE_TWO sizeof(TYPE_TWO)
#define SIZE_THREE sizeof(TYPE_THREE)
#define SIZE_FOUR 14
#define SIZE_FIVE sizeof(TYPE_FIVE)

#define VALUE_ONE 255
#define VALUE_TWO 29586
#define VALUE_THREE 123123123123123
#define VALUE_FOUR "Hello, world."
#define VALUE_FIVE 89.23f

void write() {
    FILE* out = fopen("myBinaryFile.bin", "wb");

    if (out == NULL) {
        perror("Could not open writing file.\n");
        exit(1);
    }

    TYPE_ONE valOne = VALUE_ONE;
    fwrite(&valOne, SIZE_ONE, 1, out);

    TYPE_TWO valTwo = VALUE_TWO;
    fwrite(&valTwo, SIZE_TWO, 1, out);

    TYPE_THREE valThree = VALUE_THREE;
    fwrite(&valThree, SIZE_THREE, 1, out);

    TYPE_FOUR valFour[] = VALUE_FOUR;
    fputs(valFour, out);

    TYPE_FIVE valFive = VALUE_FIVE;
    fwrite(&valFive, SIZE_FIVE, 1, out);

    if (fclose(out)) {
        printf("Could not close writing file.\n");
        exit(1);
    }
}

void read() {
    FILE* in = fopen("myBinaryFile.bin", "rb");

    if (!in) {
        printf("Could not open reading file.\n");
        exit(1);
    }

    TYPE_ONE valOne;
    fread(&valOne, SIZE_ONE, 1, in);
    printf("valOne: " F_SPEC_ONE "\n", valOne);
    if (valOne != VALUE_ONE)
        printf("valOne does not match.\n");
    
    TYPE_TWO valTwo;
    fread(&valTwo, SIZE_TWO, 1, in);
    printf("valTwo: " F_SPEC_TWO "\n", valTwo);
    if (valTwo != VALUE_TWO)
        printf("valTwo does not match.\n");

    TYPE_THREE valThree;
    fread(&valThree, SIZE_THREE, 1, in);
    printf("valThree: " F_SPEC_THREE "\n", valThree);
    if (valThree != VALUE_THREE)
        printf("valThree does not match.\n");

    TYPE_FOUR valFour[SIZE_FOUR];
    fgets(valFour, SIZE_FOUR, in);
    printf("valFour: " F_SPEC_FOUR "\n", valFour);
    if (strcmp(valFour, VALUE_FOUR))
        printf("valFour does not match.\n");

    TYPE_FIVE valFive;
    fread(&valFive, SIZE_FIVE, 1, in);
    printf("valFive: " F_SPEC_FIVE "\n", valFive);
    if (valFive != VALUE_FIVE)
        printf("valFive does not match.\n");

    if (fclose(in)) {
        printf("Could not close reading file.\n");
        exit(1);
    }
}

int main() {
    write();
    read();
}
