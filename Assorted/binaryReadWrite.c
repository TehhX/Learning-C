#include <stdio.h>
#include <stdlib.h>

#define TYPE_ONE unsigned char
#define TYPE_TWO int
#define TYPE_THREE unsigned long long

#define F_SPEC_ONE "%d"
#define F_SPEC_TWO "%d"
#define F_SPEC_THREE "%llu"

#define SIZE_ONE 1
#define SIZE_TWO sizeof(int)
#define SIZE_THREE sizeof(unsigned long long)

#define VALUE_ONE 255
#define VALUE_TWO 29586
#define VALUE_THREE 123123123123123

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

    if (fclose(out)) {
        perror("Could not close writing file.\n");
        exit(1);
    }
}

void read() {
    FILE* in = fopen("myBinaryFile.bin", "rb");

    if (in == NULL) {
        perror("Could not open reading file.\n");
        exit(1);
    }

    TYPE_ONE valOne;
    fread(&valOne, SIZE_ONE, 1, in);
    printf("valOne: " F_SPEC_ONE "\n", valOne);
    if (valOne != VALUE_ONE)
        perror("valOne does not match.\n");
    
    TYPE_TWO valTwo;
    fread(&valTwo, SIZE_TWO, 1, in);
    printf("valTwo: " F_SPEC_TWO "\n", valTwo);
    if (valTwo != VALUE_TWO)
        perror("valTwo does not match.\n");

    TYPE_THREE valThree;
    fread(&valThree, SIZE_THREE, 1, in);
    printf("valThree: " F_SPEC_THREE "\n", valThree);
    if (valThree != VALUE_THREE)
        perror("valThree does not match.\n");
    
    if (fclose(in)) {
        perror("Could not close reading file.\n");
        exit(1);
    }
}

int main() {
    write();
    read();
}
