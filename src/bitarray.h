/*
 * FILE:        bitarray.h
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Function prototypes for manipulating bit arrays.
 */

#ifndef BITARRAY_H
#define BITARRAY_H

#include <stdio.h>

struct bitarray * new_bitarray(const unsigned long);
void delete_bitarray(struct bitarray **);
void set_all_bits(struct bitarray *);
void clear_bit(struct bitarray *, const unsigned long);
int get_bit(struct bitarray *, const unsigned long);

#endif
