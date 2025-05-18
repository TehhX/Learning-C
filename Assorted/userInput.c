#include <stdio.h>

int main() {
    printf("Enter an integer: ");

    int userInputInt;
    scanf("%d", &userInputInt); // Uses the address of an integer variable with address operator, same syntax as C++
    printf("You entered: %d\n", userInputInt);
}
