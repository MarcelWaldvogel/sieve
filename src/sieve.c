/*******************************************************************************
FILE:           sieve.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-31
DESCRIPTION:    Implementation of the sieve of Eratosthenes with wheel
                factorization.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitarray.h"
#include "wheel.h"

static const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
static const unsigned long numBasePrimes = 6;

/*******************************************************************************
FUNCTION NAME:  sieve
DESCRIPTION:    Sieve of Eratosthenes algorithm implementation using wheel
                factorization. All the prime numbers less than or equal to a
                specified nonnegative integer `max' are found and printed to a
                file stream, and the number of such primes is returned. First,
                the even prime 2 is sieved if max >= 2. Then, a bit array is
                created, with one bit for every odd integer between 0 and max.
                The bit represents whether the odd integer is prime (1) or
                composite (0). The list of base primes is sieved first, and
                their multiples are marked as composite in the bit array. Then
                the remaining odd primes less than or equal to sqrt(max) are
                sieved, and their multiples are marked as composite in the bit
                array. Finally, the remaining primes between sqrt(max) and max
                are sieved.
PARAMETERS:     max (const unsigned long): the upper bound for the sieve.
                stream (FILE *): where to print the primes.
RETURNS:        The number of primes less than or equal to max.
*******************************************************************************/
static unsigned long sieve(const unsigned long max, FILE *stream) {
    BitArray *isPrime;      /* Bit array of T/F values */
    Wheel *wheel;           /* The wheel used in the sieve */
    unsigned long prime;    /* A prime candidate */
    unsigned long comp;     /* A necessarily composite number */
    unsigned long count;    /* The number of primes */
    unsigned long index;    /* Track position in loops */

    /* Deal with easy cases first */
    if (max < 2) {
        return 0;
    }
    if (stream) fprintf(stream, "2\n");
    count = 1;
    if (max == 2) {
        return 1;
    }
    /* Now max is guaranteed to be at least 3 */

    /* Create the sieving array and the wheel. To save space we only store */
    /* the primality of odd integers, starting with 1 in position 0, 3 in */
    /* position 1, etc. */
    isPrime = newBitArray((max + 1) / 2);
    if (!isPrime) {
        fprintf(stderr, "sieve: could not allocate bit array memory.\n");
        return 0;
    }
    setAllBits(isPrime, (max + 1) / 2); /* Initialize all bits to 1 */
    clearBit(isPrime, 0); /* 1 is not prime */
    wheel = newWheel(basePrimes, numBasePrimes);

    /* Sieve the odd base primes */
    for (index = 1; index < numBasePrimes; index++) {
        prime = basePrimes[index];
        if (prime > max)
            break;
        count++;
        if (stream) fprintf(stream, "%lu\n", prime);
        /* Cross off multiples of the current prime */
        for (comp = prime * prime; comp <= max; comp += 2 * prime)
            clearBit(isPrime, comp / 2);
    }

    /* Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (getBit(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, "%lu\n", prime);
            /* Cross off multiples of the current prime */
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                clearBit(isPrime, comp / 2);
        }
    }

    /* Sieve the remaining primes > sqrt(max) */
    for (; prime <= max; prime = nextp(wheel)) {
        if (getBit(isPrime, prime / 2)) {
            count++;
            if (stream) fprintf(stream, "%lu\n", prime);
        }
    }

    /* Clean up and return */
    deleteBitArray(&isPrime);
    deleteWheel(&wheel);
    return count;
}

/* Print a usage message to the user */
static void helpmessage(const char *name) {
    fprintf(stderr, "Print all the primes up to a given positive integer.\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\t%s N\n", name);
    fprintf(stderr, "\t%s [flags] N\n", name);
    fprintf(stderr, "\twhere N is a positive integer.\n");
    fprintf(stderr, "Flags:\n");
    fprintf(stderr, "\t-n\tPrint only the number of primes.\n");
    fprintf(stderr, "\t-h\tShow usage information.\n");
}

/* Run the program */
int main(int argc, const char **argv) {
    const char *name = argv[0];     /* The program name */
    unsigned long max;              /* The upper bound for the sieve */
    int opt_count = 0;              /* Whether to print the number of primes */
    char c;                         /* A command line argument char */

    /* Process command-line options */
    while (--argc > 0 && **++argv == '-') {
        while ((c = *++*argv)) {
            switch (c) {
                case 'h':
                    helpmessage(name);
                    return 0;
                case 'n':
                    opt_count = 1;
                    break;
                default:
                    fprintf(stderr, "sieve: illegal option '%c'\n", c);
                    return 1;
            }
        }
    }

    /* There should be one command-line argument remaining */
    /* If not, print a usage message */
    if (argc != 1) {
        fprintf(stderr, "sieve: expected argument.\n");
        fprintf(stderr, "For help, run %s -h\n", name);
        return 1;
    }

    max = strtoul(*argv, NULL, 10);
    if (opt_count)
        printf("%lu\n", sieve(max, NULL));
    else
        sieve(max, stdout);

    /* Clean up and return */
    return 0;
}
