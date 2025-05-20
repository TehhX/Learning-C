#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // malloc(...) reserves memory, and returns a void ptr to the begining of the reserved memory. It does not initialize any values.
    float* fPtrM = malloc(10 * sizeof(float));
    fPtrM[0] = 3.14;

    // malloc returns NULL if it fails
    if (fPtrM == NULL) {
        printf("The memory could not be allocated.\n");
        return 1;
    }
    else
        printf("Malloc: %f\n", fPtrM[0]);

    // calloc(...) does just the same thing as malloc(...), but initializes all reserved memory to 0
    int* iPtrC = calloc(1, sizeof(int));

    if (iPtrC == NULL) {
        printf("The memory could not be allocated.\n");
        return 1;
    }
    else
        printf("Calloc: %d\n", *iPtrC);

    // free(...) frees dynam. allocated memory
    free(fPtrM);
    free(iPtrC);

    printf("Gibberish of freed ptr: %f\n", fPtrM[0]);

    // realloc(...) can change the size of a previously allocated chunk of memory
    char* name = malloc(4 * sizeof(char));

    // 3 characters and a null terminator is 4 characters
    strcpy(name, "Bob");
    printf("Name: %s\n", name);

    name = realloc(name, 6 * sizeof(char));

    // strcopy(dest, src) copies a string from ptr to ptr
    strcpy(name, "Timmy");
    printf("Name: %s\n", name);

    free(name);

    return 0;
}
