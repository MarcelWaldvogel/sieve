/*
 * FILE:        bitarray.c
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Implementation of bit arrays
 */

#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "bitarray.h"

#ifdef DEBUG_ON
#include "debug.h"
#endif

/*
 * STRUCT:      bitarray
 * DESCRIPTION: Implementation of bit arrays, where each bit can be individually
 *              manipulated.
 * FIELDS:      array (int *): An array of ints which contains the bits.
 *              size (size_t): The number of bytes in the bit array.
 */
struct bitarray {
    int *array;
    size_t size;
};

/* Number of bits in an element of a BitArray */
#define NBITS (CHAR_BIT * sizeof(int))

/*
 * FUNCTION:    new_bitarray
 * DESCRIPTION: Allocates memory for a new bit array holding at least the
 *              specified number of bits.
 * ERRORS:      If memory allocation fails, returns NULL.
 * PARAMETERS:  n (const unsigned long): The number of bits to store.
 * RETURNS:     A new bit array with at least n bits.
 */
struct bitarray * new_bitarray(const unsigned long n) {
    /* How many bytes to store */
    size_t bytes = (n + CHAR_BIT - 1) / CHAR_BIT;

    struct bitarray *bits = malloc(sizeof(struct bitarray));
    if (!bits)  /* Check if malloc failed */
        goto failure;

    /* Compute number of bytes needed to hold enough ints */
    bits->size = ((bytes + sizeof(int) - 1) / sizeof(int)) * sizeof(int);

    bits->array = malloc(bits->size);
    if (!bits->array)
        goto failure;

#ifdef DEBUG_ON
    DEBUG_MSG("New bit array at %p (bytes: %lu)", (void *) bits, bits->size);
#endif

    return bits;

failure:
    delete_bitarray(&bits);
    return NULL;
}

/*
 * FUNCTION:    delete_bitarray
 * DESCRIPTION: Deallocate memory associated with a bit array.
 * PARAMETERS:  bpp (struct bitarray **): Pointer to a pointer to the bit array
 *              to be deleted.
 * RETURNS:     Nothing.
 */
void delete_bitarray(struct bitarray **bpp) {
    if (bpp && *bpp) {
#ifdef DEBUG_ON
        DEBUG_MSG("Deleting bit array at %p ...", (void *) *bpp);
#endif
        if ((*bpp)->array)
            free((*bpp)->array);
        free(*bpp);
        *bpp = NULL;
    }
}

/*
 * FUNCTION:    set_all_bits
 * DESCRIPTION: Sets all bits in the bit array to 1.
 * PARAMETERS:  bits (struct bitarray *): Pointer to the bit array to operate
 *              on. No error checking is done to ensure bits is not NULL.
 * RETURNS:     Nothing.
 */
void set_all_bits(struct bitarray *bits) {
    memset(bits->array, (1UL << CHAR_BIT) - 1, bits->size);
}

/*
 * FUNCTION:    clear_bit
 * DESCRIPTION: Sets the kth bit of a bit array to 0.
 * PARAMETERS:  bits (struct bitarray *): The bit array whose kth bit is to be
 *              set to 0.
 *              k (const unsigned long): The position of the bit to be cleared
 * RETURNS:     Nothing.
 */
void clear_bit(struct bitarray *bits, const unsigned long k) {
    bits->array[k / NBITS] &= ~(1 << (k % NBITS));
}

/*
 * FUNCTION:    get_bit
 * DESCRIPTION: Get the kth bit of a bit array
 * PARAMETERS:  bits (struct bitarray *): The bit array whose kth bit is to be
 *              returned.
 *              k (const unsigned long): The position of the bit to be returned
 * RETURNS:     The kth bit in the bit array
 */
int get_bit(struct bitarray *bits, const unsigned long k) {
    return (bits->array[k / NBITS] & (1 << (k % NBITS))) != 0;
}
