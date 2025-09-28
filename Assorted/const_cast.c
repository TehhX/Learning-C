#include "stdio.h"

int get_val(const int *ptr) {
    return *ptr;
}

int main() {
    const int var_const = 1;
    printf("var_const before: %d\n", var_const);

    volatile const int var_vol_const = 2;
    printf("var_vol_const before: %d\n", var_vol_const);

    // Notice that the constant values cannot be changed (Compile time error) (Try uncommenting them):
    // var_const = -1;
    // var_vol_const = -2;

    // Use a cast from "const int *" to "int *" to modify it instead.
    int *var_const_ptr = (int *) &var_const;
    *var_const_ptr = -3;

    // Another method without requiring another variable:
    *(int *) &var_const = -5;

    // Volatile keyword in type and cast not required, but useful for clarity.
    volatile int *var_vol_const_ptr = (volatile int *) &var_vol_const;
    *var_vol_const_ptr = -4;

    // Will still print 1 on -O3 as variable doesn't know it has been changed, but main() has cached its value (perhaps the value remains in the register, not requiring a reread?).
    printf("var_const after: %d\n", var_const);

    // Will print -4 on -O3 due to volatile keyword requiring a value re-read every access.
    printf("var_vol_const after: %d\n", var_vol_const);

    // Will print -3 on -O3 as value is reread by get_val due to a difference of scope (I believe).
    printf("var_const after, using get_val: %d\n", get_val(&var_const));
}
