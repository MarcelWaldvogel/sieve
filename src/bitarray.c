/*******************************************************************************
FILE:           bitarray.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-31
DESCRIPTION:    Implementation of bit arrays.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "bitarray.h"

#define NUMBITS (8 * sizeof(int)) /* # of bits in each element of a BitArray */

/*******************************************************************************
FUNCTION NAME:  newBitArray
DESCRIPTION:    Allocates memory for a new bit array holding at least the
                specified number of bits.
PARAMETERS:     n (const unsigned long): the number of bits to store.
RETURNS:        A new bit array with at least n bits.
*******************************************************************************/
BitArray * newBitArray(const unsigned long n) {
    unsigned long size = (n + sizeof(int) - 1) / sizeof(int);
    BitArray *bits = malloc(size);
    return bits; /* Could be NULL if malloc fails */
}

/*******************************************************************************
FUNCTION NAME:  deleteBitArray
DESCRIPTION:    Deallocate memory associated with a bit array
PARAMETERS:     bpp (BitArray **) Pointer to a pointer to the bit array to
                delete.
RETURNS:        Nothing.
*******************************************************************************/
void deleteBitArray(BitArray **bpp) {
    if (bpp && *bpp) {
        free(*bpp);
        *bpp = NULL;
    }
}

/*******************************************************************************
FUNCTION NAME:  setAllBits
DESCRIPTION:    Sets all bits in the bit array to 1.
PARAMETERS:     bits (BitArray *): the bit array to operate on.
                n (const unsigned long): the number of bits in the bit array.
RETURNS:        Nothing.
*******************************************************************************/
void setAllBits(BitArray *bits, const unsigned long n) {
    unsigned long size = (n + sizeof(int) - 1) / sizeof(int);
    memset(bits, 0xff, size); /* 0xff is a byte of 1s */
}

/*******************************************************************************
FUNCTION NAME:  clearBit
DESCRIPTION:    Sets the bit at position k to 0.
PARAMETERS:     bits (BitArray *): the bit array to operate on.
                k (const unsigned long): the position of the bit to turn off.
RETURNS:        Nothing.
*******************************************************************************/
void clearBit(BitArray *bits, const unsigned long k) {
    bits[k / NUMBITS] &= ~(1 << (k % NUMBITS));
}

/*******************************************************************************
FUNCTION NAME:  getBit
DESCRIPTION:    Returns the bit at position k
PARAMETERS:     bits (BitArray *): the bit array to operate on.
                k (const unsigned long): the position of the bit to return
RETURNS:        The bit at position k.
*******************************************************************************/
int getBit(BitArray *bits, const unsigned long k) {
    return (bits[k / NUMBITS] & (1 << (k % NUMBITS))) != 0;
}
