// Checks if argv[1] contains PHRASE by converting it into a 4 byte value. Assumes all user input is correct. Input should always have first argument 4 bytes (characters) long. Not a safe program whatsoever

#include "stdio.h"
#include "stdint.h"

// TODO: May only work on little-endian systems
#if !defined(__BYTE_ORDER__) || (__BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__)
    #error System not little-endian, or endianness unknown. Feel free to comment this out and test it yourself, unsure what will happen
#endif

// The phrase to check against (Must be 8 characters long)
#define PHRASE "ABCDEFGH"

// Base definition
#define _STR_MATCHES_BASE(STR1, STR2, MASK, TYPE) ((*((TYPE *) (STR1)) & (MASK)) == (*((TYPE *) (STR2)) & (MASK)))

// Checks if both strings match for given bytes from start
#define STR_MATCHES_1B(STR1, STR2) ((STR1)[0] == (STR2)[0])
#define STR_MATCHES_2B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000000000000000000000000000000000000000000001111111111111111, uint16_t))
#define STR_MATCHES_3B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000000000000000000000000000000000000111111111111111111111111, uint32_t))
#define STR_MATCHES_4B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000000000000000000000000000011111111111111111111111111111111, uint32_t))
#define STR_MATCHES_5B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000000000000000000001111111111111111111111111111111111111111, uint64_t))
#define STR_MATCHES_6B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000000000000111111111111111111111111111111111111111111111111, uint64_t))
#define STR_MATCHES_7B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b0000000011111111111111111111111111111111111111111111111111111111, uint64_t))
#define STR_MATCHES_8B(STR1, STR2) (_STR_MATCHES_BASE(STR1, STR2, 0b1111111111111111111111111111111111111111111111111111111111111111, uint64_t))

// Output magic
#define OUTPUT_MATCH(BYTES) (STR_MATCHES_##BYTES##B(argv[1], PHRASE) ? "MATCH" : "NOT MATCH")

int main(__attribute__((unused)) int argc, char **argv)
{
    printf("1: %s\n2: %s\n3: %s\n4: %s\n5: %s\n6: %s\n7: %s\n8: %s", OUTPUT_MATCH(1), OUTPUT_MATCH(2), OUTPUT_MATCH(3), OUTPUT_MATCH(4), OUTPUT_MATCH(5), OUTPUT_MATCH(6), OUTPUT_MATCH(7), OUTPUT_MATCH(8));
}
