/*
 * FILE:        bitarray.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-05-19
 * DESCRIPTION: Function prototypes for manipulating bit arrays.
 */

#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdio.h>

typedef struct BitArray BitArray;

BitArray * newBitArray(const unsigned long);
void deleteBitArray(BitArray **);
void setAllBits(BitArray *);
void clearBit(BitArray *, const unsigned long);
int getBit(BitArray *, const unsigned long);

#endif
