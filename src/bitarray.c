/*
 * FILE:        bitarray.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-05-19
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
 * STRUCT:      BitArray
 * DESCRIPTION: Implementation of bit arrays, where each bit can be individually
 *              manipulated.
 * FIELDS:      array (int *): An array of ints which contains the bits.
 *              size (size_t): The number of bytes in the bit array.
 */
struct BitArray {
    int *array;
    size_t size;
};

/* Number of bits in an element of a BitArray */
#define NBITS (CHAR_BIT * sizeof(int))

/*
 * FUNCTION:    newBitArray
 * DESCRIPTION: Allocates memory for a new bit array holding at least the
 *              specified number of bits.
 * ERRORS:      If memory allocation fails, returns NULL.
 * PARAMETERS:  n (const unsigned long): The number of bits to store.
 * RETURNS:     A new bit array with at least n bits.
 */
BitArray * newBitArray(const unsigned long n) {
    /* How many bytes to store */
    size_t bytes = (n + CHAR_BIT - 1) / CHAR_BIT;

    BitArray *bits = malloc(sizeof(BitArray));  /* Allocate bit array memory */
    if (!bits)                                  /* Check if malloc failed */
        return NULL;

    /* Compute number of bytes needed to hold enough ints */
    bits->size = ((bytes + sizeof(int) - 1) / sizeof(int)) * sizeof(int);

    bits->array = malloc(bits->size);
    if (!bits->array) {
        deleteBitArray(&bits);
        return NULL;
    }

    #ifdef DEBUG_ON
    DEBUG_MSG("New bit array at %p (bytes: %lu)", (void *) bits, bits->size);
    #endif

    return bits;
}

/*
 * FUNCTION:    deleteBitArray
 * DESCRIPTION: Deallocate memory associated with a bit array.
 * PARAMETERS:  bpp (BitArray **): Pointer to a pointer to the bit array to
 *              delete.
 * RETURNS:     Nothing.
 */
void deleteBitArray(BitArray **bpp) {
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
 * FUNCTION:    setAllBits
 * DESCRIPTION: Sets all bits in the bit array to 1.
 * PARAMETERS:  bits (BitArray *): The bit array to operate on.
 *              n (const unsigned long): The number of bits in the bit array.
 * RETURNS:     Nothing.
 */
void setAllBits(BitArray *bits) {
    memset(bits->array, (1L << CHAR_BIT) - 1, bits->size);
}

/*
 * FUNCTION:    clearBit
 * DESCRIPTION: Sets the kth bit of a bit array to 0.
 * PARAMETERS:  bits (BitArray *): The bit array whose kth bit is to be
 *              set to 0.
 *              k (const unsigned long): The position of the bit to be cleared
 * RETURNS:     Nothing.
 */
void clearBit(BitArray *bits, const unsigned long k) {
    bits->array[k / NBITS] &= ~(1 << (k % NBITS));
}

/*
 * FUNCTION:    getBit
 * DESCRIPTION: Get the kth bit of a bit array
 * PARAMETERS:  bits (BitArray *): The bit array whose kth bit is to be
 *              returned.
 *              k (const unsigned long): The position of the bit to be returned
 * RETURNS:     The kth bit in the bit array
 */
int getBit(BitArray *bits, const unsigned long k) {
    return (bits->array[k / NBITS] & (1 << (k % NBITS))) != 0;
}
