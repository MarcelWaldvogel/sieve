/*******************************************************************************
FILE:           sieve.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-15
DESCRIPTION:    Implementation of the sieve of Eratosthenes with wheel
                factorization.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wheel.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
const unsigned long numBasePrimes = 6;

void sieve(unsigned long num) {
    /* Step 0 -- Local variable declarations */
    unsigned char *isPrime; /* Array of T/F values */
    Wheel *wheel;           /* The wheel used in the sieve */
    unsigned long prime;    /* A prime candidate */
    unsigned long comp;     /* A necessarily composite number */

    /* Step 1 -- Create the sieving array */
    isPrime = (unsigned char *) malloc((num + 1) * sizeof(unsigned char));
    memset(isPrime, TRUE, (num + 1) * sizeof(unsigned char));
    isPrime[0] = isPrime[1] = FALSE;

    /* Step 2 -- Sieve out the base primes */
    for (unsigned long i = 0; i < numBasePrimes; i++) {
        prime = basePrimes[i];
        if (prime > num)
            break;
        printf("%lu\n", prime);
        for (comp = prime * prime; comp <= num; comp += prime) {
            isPrime[comp] = FALSE;
        }
    }

    /* Step 3 -- Create the wheel for wheel factorization */
    wheel = newWheel(basePrimes, numBasePrimes);
    nextp(wheel); /* Ignore 1 */

    /* Step 4 -- Sieve the remaining primes <= sqrt(num) */
    for (prime = nextp(wheel); prime * prime <= num; prime = nextp(wheel)) {
        if (isPrime[prime]) {
            printf("%lu\n", prime);
            for (comp = prime * prime; comp <= num; comp += 2 * prime) {
                isPrime[comp] = FALSE;
            }
        }
    }

    /* Step 5 -- Sieve the remaining primes > sqrt(num) */
    for (; prime <= num; prime = nextp(wheel)) {
        if (isPrime[prime]) {
            printf("%lu\n", prime);
        }
    }

    /* Step 6 -- Clean up */
    free(isPrime);
    deleteWheel(&wheel);
}

int main(int argc, const char **argv) {
    if (argc == 1) {
        fprintf(stderr,
            "Usage: Enter a positive integer: e.g.\n%s 12345\n", argv[0]);
        return 1;
    }

    sieve(strtoul(*(++argv), NULL, 10));
    return 0;
}
