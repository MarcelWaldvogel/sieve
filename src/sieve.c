/* 
 * FILE:        sieve.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-03-24
 * DESCRIPTION: Implementation of the sieve of Eratosthenes with wheel
 *              factorization.
 */

#include <stdio.h>
#include "bitarray.h"
#include "wheel.h"
#include "sieve.h"

#define ERR_BIT_ALLOCATE    "sieve: could not allocate bit array memory.\n"
#define PRIME_FORMAT        "%lu\n"

static const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
static const unsigned long numBasePrimes = 6;

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
    BitArray *isPrime;      /* Bit array of T/F values */
    Wheel *wheel;           /* The wheel used in the sieve */
    unsigned long prime;    /* A prime candidate */
    unsigned long comp;     /* A necessarily composite number */
    unsigned long count;    /* The number of primes */
    unsigned long index;    /* Track position in loops */

    /* Create the sieving array and the wheel. To save space we only store */
    /* the primality of odd integers, starting with 1 in position 0, 3 in */
    /* position 1, etc. */
    isPrime = newBitArray((max + 1) / 2);
    if (!isPrime) {
        fprintf(stderr, ERR_BIT_ALLOCATE);
        return 0;
    }
    setAllBits(isPrime, (max + 1) / 2); /* Initialize all bits to 1 */
    clearBit(isPrime, 0); /* 1 is not prime */
    wheel = newWheel(basePrimes, numBasePrimes);

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
            clearBit(isPrime, comp / 2);
    }

    /* Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (getBit(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, PRIME_FORMAT, prime);
            /* Cross off multiples of the current prime */
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                clearBit(isPrime, comp / 2);
        }
    }

    /* Sieve the remaining primes > sqrt(max) */
    for (; prime <= max; prime = nextp(wheel)) {
        if (getBit(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, PRIME_FORMAT, prime);
        }
    }

    /* Clean up and return */
    deleteBitArray(&isPrime);
    deleteWheel(&wheel);
    return count;
}
