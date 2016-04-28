/*
 * FILE:        bitarray.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-28
 * DESCRIPTION: Function prototypes for manipulating bit arrays.
 */

#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdio.h>

typedef struct BitArray BitArray;

BitArray * newBitArray(const unsigned long);
void deleteBitArray(BitArray **);
void setAllBits(BitArray *) __attribute__((nonnull));
void clearBit(BitArray *, const unsigned long) __attribute__((hot, nonnull));
int getBit(BitArray *, const unsigned long) __attribute__((hot, nonnull));

#endif
