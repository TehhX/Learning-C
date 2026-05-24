// Returns pointer to a requested datum. Also tells where an element *would* go, making it a warranted improvement on the standard library's `bsearch(...)` which will return NULL if it's not found

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"

enum binary_search_status
{
    binary_search_status_found, // Requested value was found at returned address
    binary_search_status_above, // Requested value was not found, but would be placed one datum above returned address if it were
    binary_search_status_start  // Requested value was not found, but would be placed at the beginning
};

struct binary_search_return
{
    void *address;
    enum binary_search_status status;
};

typedef int (*binary_search_compare_t)(const void *datum_a, const void *datum_b);

static struct binary_search_return binary_search_perform(const void *const needle, void *const haystack, const size_t size, const size_t count, binary_search_compare_t data_compare)
{
    // Check if it goes past the array
    if (data_compare(needle, ((uint8_t *) haystack) + size * (count - 1)) > 0)
    {
        return (struct binary_search_return){ .address = ((uint8_t *) haystack) + size * (count - 1), .status = binary_search_status_above };
    }
    // Check if it goes before the array
    else if (data_compare(needle, haystack) < 0)
    {
        return (struct binary_search_return){ .address = haystack, .status = binary_search_status_start };
    }

    // Begin a binary search to find where it goes/should go in the array
    size_t low = 0, high = count - 1, mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;

        const int difference = data_compare(((uint8_t *) haystack) + size * mid, needle);

        if (difference == 0)
        {
            return (struct binary_search_return){ .address = (uint8_t *) (haystack) + size * mid, .status = binary_search_status_found };
        }
        else if (difference < 0)
        {
            low = mid + 1;
        }
        else if (difference > 0)
        {
            high = mid - 1;
        }
    }

    return (struct binary_search_return){ .address = (uint8_t *) (haystack) + size * (mid - 1), .status = binary_search_status_above };
}

static void binary_search_return_print(const struct binary_search_return *const binary_search_return, const size_t size, const void *const haystack)
{
    switch (binary_search_return->status)
    {
        break; case binary_search_status_found:
        {
            printf("Found at index %zu.\n", ((size_t) (((uint8_t *) binary_search_return->address) - ((uint8_t *) haystack))) / size);
        }
        break; case binary_search_status_above:
        {
            printf("Would be above index %zu.\n", ((size_t) (((uint8_t *) binary_search_return->address) - ((uint8_t *) haystack))) / size);
        }
        break; case binary_search_status_start:
        {
            printf("Would be at start.\n");
        }
    }
}

// Note: Using -1,0,+1 because the return value might overflow and its exact difference is inconsequential. Keep this in mind if using in any real programs
static int int_compare(const int *const a, const int *const b)
{
    if (*a >= *b)
    {
        if (*a > *b)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

struct person
{
    size_t id;
    char name[64];
};

// Note: Using -1,0,+1 because the return value might overflow and its exact difference is inconsequential. Keep this in mind if using in any real programs
static int person_compare(const struct person *const a, const struct person *const b)
{
    if (a->id >= b->id)
    {
        if (a->id > b->id)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

static int char_compare(const unsigned char *const a, const unsigned char *const b)
{
    if (*a >= *b)
    {
        if (*a > *b)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

int main()
{
    // Declare, populate integers
    int integers[120];
    for (int i = 0; i < (int) (sizeof(integers) / sizeof(*integers)); ++i)
    {
        integers[i] = i * 4;
    }

    // Search, report finding on integers
    const int search_int = 119 * 4 + 1;
    struct binary_search_return int_return = binary_search_perform(&search_int, integers, sizeof(*integers), sizeof(integers) / sizeof(*integers), (binary_search_compare_t) int_compare);
    binary_search_return_print(&int_return, sizeof(int), integers);

    // Declare, populate people (no pun intended)
    struct person people[25];
    for (int i = 0; i < (int) (sizeof(people) / sizeof(*people)); ++i)
    {
        memcpy(people + i, &(struct person){ .id = (i + 1) * 10000, .name = "Howitzer Jackson" }, sizeof(*people));
    }

    // Search, report finding on people
    const struct person search_person = { .id = 250001, .name = "Fowl" };
    struct binary_search_return person_return = binary_search_perform(&search_person, people, sizeof(*people), sizeof(people) / sizeof(*people), (binary_search_compare_t) person_compare);
    binary_search_return_print(&person_return, sizeof(struct person), people);

    // Declare, populate characters
    unsigned char characters[256];
    for (int i = 0; i < (int) (sizeof(characters) / sizeof(*characters)); ++i)
    {
        characters[i] = i;
    }

    // Search, report finding on integers
    const unsigned char search_char = 'x';
    struct binary_search_return char_return = binary_search_perform(&search_char, characters, sizeof(*characters), sizeof(characters) / sizeof(*characters), (binary_search_compare_t) char_compare);
    binary_search_return_print(&char_return, sizeof(*characters), characters);
}
