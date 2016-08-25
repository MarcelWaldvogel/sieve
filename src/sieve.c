/*
 * FILE:        sieve.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-08-24
 * DESCRIPTION: Implementation of the sieve of Eratosthenes with wheel
 *              factorization. This creates one of two object files: if the
 *              macro COUNT_PRIMES is defined, it creates an object file
 *              containing the function sieve_count, which returns the number of
 *              primes up to a given number. If the macro LIST_PRIMES is
 *              defined, it creates an object file containing the function
 *              sieve_list, which prints the primes up to a given number to
 *              stdout. Exactly one of these two macros must be defined.
 */

/* Ensure that just one of the macros COUNT_PRIMES and LIST_PRIMES is defined */
#if !defined(COUNT_PRIMES) && !defined(LIST_PRIMES)
#error "One of the macros COUNT_PRIMES and LIST_PRIMES must be defined."
#endif
#if defined COUNT_PRIMES && defined LIST_PRIMES
#error "Both macros COUNT_PRIMES and LIST_PRIMES cannot be defined."
#endif

#include <stdlib.h>
#include <stdio.h>

#include "bitarray.h"
#include "wheel.h"
#include "sieve.h"

#define ERR_BIT_ALLOCATE    "sieve: bit array"
#define ERR_WHEEL_ALLOCATE  "sieve: wheel"

static const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
static const unsigned long numBasePrimes = 6;

#ifndef COUNT_PRIMES
static void printul(unsigned long);
#endif

/*
 * FUNCTIONS:   sieve_count/sieve_list
 * DESCRIPTION: Sieve of Eratosthenes algorithm implementation using wheel
 *              factorization. All the prime numbers less than or equal to a
 *              specified nonnegative integer `max' are found and either
 *              (sieve_list) printed to stdout, or (sieve_count) the number of
 *              such primes is returned.
 *              First, the prime 2 is sieved if max >= 2. Then, a bit array is
 *              created, with one bit for every odd integer between 0 and max.
 *              The bit represents whether the odd integer is prime (1) or
 *              composite (0). The list of base primes is sieved first, and
 *              their multiples are marked as composite in the bit array. Then
 *              the remaining odd primes less than or equal to sqrt(max) are
 *              sieved, and their multiples are marked as composite in the bit
 *              array. Finally, the remaining primes between sqrt(max) and max
 *              are sieved.
 * PARAMETERS:  max (const unsigned long): The upper bound for the sieve.
 * RETURNS:     sieve_count: The number of primes less than or equal to max.
 *              sieve_list: Nothing.
 */
#ifdef COUNT_PRIMES
unsigned long sieve_count(const unsigned long max)
#else
void sieve_list(const unsigned long max)
#endif
{
    BitArray *isPrime = NULL;   /* Bit array representing primality */
    Wheel *wheel = NULL;        /* The wheel used in the sieve */
#ifdef COUNT_PRIMES
    unsigned long count = 0;    /* The number of primes */
#endif
    unsigned long prime;        /* A prime candidate */
    unsigned long comp;         /* A necessarily composite number */
    unsigned long index;        /* Track position in loops */

    /* Create the sieving array. To save space we only store the primality of
     * odd integers, starting with 1 in position 0, 3 in position 1, etc. */
    isPrime = newBitArray((max + 1) / 2);
    if (!isPrime) {
        perror(ERR_BIT_ALLOCATE);
        goto failure;
    }
    setAllBits(isPrime);    /* Initialize all bits to 1 */
    clearBit(isPrime, 0);   /* 1 is not prime */

    /* Create the wheel */
    wheel = newWheel(basePrimes, numBasePrimes);
    if (!wheel) {
        perror(ERR_WHEEL_ALLOCATE);
        goto failure;
    }

    /* Easy case #1: no primes less than 2 */
    if (max < 2)
        goto end;

    /* Now max is guaranteed to be at least 2 */
#ifdef COUNT_PRIMES
    count++;
#else
    printul(2UL);
#endif

    /* Easy case #2: only the even prime */
    if (max == 2)
        goto end;

    /* Now max is guaranteed to be at least 3 */

    /* Sieve the odd base primes */
    for (index = 1; index < numBasePrimes; index++) {
        prime = basePrimes[index];
        if (prime > max)
            break;
#ifdef COUNT_PRIMES
        count++;
#else
        printul(prime);
#endif
        /* Cross off multiples of the current prime */
        for (comp = prime * prime; comp <= max; comp += 2 * prime)
            clearBit(isPrime, comp / 2);
    }

    /* Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (getBit(isPrime, prime / 2)) {
#ifdef COUNT_PRIMES
            count++;
#else
            printul(prime);
#endif
            /* Cross off multiples of the current prime */
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                clearBit(isPrime, comp / 2);
        }
    }

    /* Sieve the remaining primes > sqrt(max) */
    while(prime <= max) {
        if (getBit(isPrime, prime / 2)) {
#ifdef COUNT_PRIMES
            count++;
#else
            printul(prime);
#endif
        }
        prime = nextp(wheel);
    }

end:
    /* Clean up and return */
    deleteBitArray(&isPrime);
    deleteWheel(&wheel);
#ifdef COUNT_PRIMES
    return count;
#endif

failure:
    deleteBitArray(&isPrime);
    deleteWheel(&wheel);
    exit(EXIT_FAILURE);
}

#ifndef COUNT_PRIMES
#define BASE 10
#define TOCHAR(d) ('0' + (d))
/*
 * FUNCTION:    printul
 * DESCRIPTION: Prints an unsigned long to stdout (in decimal).
 * PARAMETERS:  n (unsigned long): The number to be printed.
 * RETURNS:     Nothing.
 */
static void printul(unsigned long n) {
    char digits[BUFSIZ];
    char *digit = digits;
    *digit++ = '\0';
    while (n) {
        *digit++ = TOCHAR(n % BASE);
        n /= BASE;
    }
    while (*--digit) {
        putchar(*digit);
    }
    putchar('\n');
}
#endif
