/*
 * FILE:        bitarray.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-27
 * DESCRIPTION: Function prototypes for manipulating bit arrays.
 */

#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdio.h>

typedef struct BitArray BitArray;

BitArray * newBitArray(const unsigned long);    /* Constructor */
void deleteBitArray(BitArray **);               /* Destructor */
void setAllBits(BitArray *);                    /* Set all bits to 1 */
void clearBit(BitArray *, const unsigned long); /* Clear a specific bit */
int getBit(BitArray *, const unsigned long);    /* Return a specific bit */
void printBitArray(BitArray *, FILE *);

#endif
