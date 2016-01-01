/*******************************************************************************
FILE:           bitarray.h
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-31
DESCRIPTION:    Interface for bit arrays.
*******************************************************************************/

#ifndef BITARRAY_H
#define BITARRAY_H

typedef int BitArray;

BitArray * newBitArray(const unsigned long);
void deleteBitArray(BitArray **);
void setAllBits(BitArray *, const unsigned long);
void clearBit(BitArray *, const unsigned long);
int getBit(BitArray *, const unsigned long);

#endif
