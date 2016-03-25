/* 
 * FILE:        sieve.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-03-24
 * DESCRIPTION: Implementation of the sieve of Eratosthenes with wheel
 *              factorization.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "wheel.h"
#include "sieve.h"

#define ERR_BIT_ALLOCATE    "sieve: could not allocate bit array memory.\n"
#define ERR_WHEEL_ALLOCATE  "sieve: could not allocate wheel memory.\n"
#define PRIME_FORMAT        "%lu\n"

static const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
static const unsigned long numBasePrimes = 6;

typedef int BitArray;

/* Number of bits in an int */
#define NBITS               (8 * sizeof(int))

/* Clear the kth bit of a bit array */
#define CLEARBIT(bits, k)   (bits)[(k) / NBITS] &= ~(1 << ((k) % NBITS))

/* Get the kth bit of a bit array */
#define GETBIT(bits, k)     ((bits)[(k) / NBITS] & (1 << ((k) % NBITS))) != 0

/*
 * FUNCTION:    newBitArray
 * DESCRIPTION: Allocates memory for a new bit array holding at least the
 *              specified number of bits.
 * ERRORS:      If memory allocation fails, returns NULL.
 * PARAMETERS:  n (const unsigned long): The number of bits to store.
 * RETURNS:     A new bit array with at least n bits.
 */
static BitArray * newBitArray(const unsigned long n) {
    unsigned long size = (n + sizeof(int) - 1) / sizeof(int);
    BitArray *bits = malloc(size);
    return bits; /* Could be NULL if malloc fails */
}

/*
 * FUNCTION:    deleteBitArray
 * DESCRIPTION: Deallocate memory associated with a bit array.
 * PARAMETERS:  bpp (BitArray **): Pointer to a pointer to the bit array to
 *              delete.
 * RETURNS:     Nothing.
 */
static void deleteBitArray(BitArray **bpp) {
    if (bpp && *bpp) {
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
static void setAllBits(BitArray *bits, const unsigned long n) {
    unsigned long size = (n + sizeof(int) - 1) / sizeof(int);
    memset(bits, 0xff, size); /* 0xff is a byte of 1s */
}

/*
 * FUNCTION:    sieve
 * DESCRIPTION: Sieve of Eratosthenes algorithm implementation using wheel
 *              factorization. All the prime numbers less than or equal to a
 *              specified nonnegative integer `max' are found and printed to a
 *              file stream, and the number of such primes is returned. First,
 *              the even prime 2 is sieved if max >= 2. Then, a bit array is
 *              created, with one bit for every odd integer between 0 and max.
 *              The bit represents whether the odd integer is prime (1) or
 *              composite (0). The list of base primes is sieved first, and
 *              their multiples are marked as composite in the bit array. Then
 *              the remaining odd primes less than or equal to sqrt(max) are
 *              sieved, and their multiples are marked as composite in the bit
 *              array. Finally, the remaining primes between sqrt(max) and max
 *              are sieved.
 * PARAMETERS:  max (const unsigned long): The upper bound for the sieve.
 *              stream (FILE *): Where to print the primes.
 * RETURNS:     The number of primes less than or equal to max.
 */
unsigned long sieve(const unsigned long max, FILE *stream) {
    BitArray *isPrime;      /* Bit array of T/F values representing primality */
    Wheel *wheel;           /* The wheel used in the sieve */
    unsigned long prime;    /* A prime candidate */
    unsigned long comp;     /* A necessarily composite number */
    unsigned long count;    /* The number of primes */
    unsigned long index;    /* Track position in loops */

    /* Create the sieving array. To save space we only store the primality of
     * odd integers, starting with 1 in position 0, 3 in position 1, etc. */
    isPrime = newBitArray((max + 1) / 2);
    if (!isPrime) {
        fprintf(stderr, ERR_BIT_ALLOCATE);
        return 0;
    }
    setAllBits(isPrime, (max + 1) / 2); /* Initialize all bits to 1 */
    CLEARBIT(isPrime, 0);               /* 1 is not prime */

    /* Create the wheel */
    wheel = newWheel(basePrimes, numBasePrimes);
    if (!wheel) {
        fprintf(stderr, ERR_WHEEL_ALLOCATE);
        return 0;
    }

    /* Initialize variables */
    count = 0;
    prime = 2;

    /* Deal with easy cases first */
    if (max < prime) {
        deleteBitArray(&isPrime);
        deleteWheel(&wheel);
        return count;
    } /* Now max is guaranteed to be at least 2 */
    count++;
    if (stream) fprintf(stream, PRIME_FORMAT, prime);
    if (max == prime) {
        deleteBitArray(&isPrime);
        deleteWheel(&wheel);
        return count;
    } /* Now max is guaranteed to be at least 3 */

    /* Sieve the odd base primes */
    for (index = 1; index < numBasePrimes; index++) {
        prime = basePrimes[index];
        if (prime > max)
            break;
        count++;
        if (stream) fprintf(stream, PRIME_FORMAT, prime);
        /* Cross off multiples of the current prime */
        for (comp = prime * prime; comp <= max; comp += 2 * prime)
            CLEARBIT(isPrime, comp / 2);
    }

    /* Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (GETBIT(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, PRIME_FORMAT, prime);
            /* Cross off multiples of the current prime */
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                CLEARBIT(isPrime, comp / 2);
        }
    }

    /* Sieve the remaining primes > sqrt(max) */
    for (; prime <= max; prime = nextp(wheel)) {
        if (GETBIT(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, PRIME_FORMAT, prime);
        }
    }

    /* Clean up and return */
    deleteBitArray(&isPrime);
    deleteWheel(&wheel);
    return count;
}
