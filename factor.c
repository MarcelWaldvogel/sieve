/*******************************************************************************
FILE:           factor.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-22
DESCRIPTION:    Prints out the prime factors of a given positive integer.
                Prime factors are computed using trial division with wheel
                factorization.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wheel.h"

const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
const unsigned long numBasePrimes = 6;

unsigned long factor(unsigned long num, unsigned char unique, FILE *stream) {
    Wheel *wheel;               /* The wheel used in the algorithm */
    unsigned long i;            /* Track position in loops */
    unsigned long count = 0;    /* The number of divisors */

    if (num > 1) {
        unsigned long prime;
        /* Divide by the base primes */
        for (i = 0; i < numBasePrimes; i++) {
            prime = basePrimes[i];
            if (unique) {
                if (num % prime == 0) {
                    if (stream) fprintf(stream, "%lu\n", prime);
                    count++;
                    for (num /= prime; num % prime == 0; num /= prime);
                }
            } else {
                while (num % prime == 0) {
                    if (stream) fprintf(stream, "%lu\n", prime);
                    num /= prime;
                    count++;
                }
            }
        }

        /* Create the wheel for wheel factorization */
        wheel = newWheel(basePrimes, numBasePrimes);
        nextp(wheel); /* Ignore 1 */
        /* Divide by the prime candiates computed by the wheel */
        while (num > 1) {
            prime = nextp(wheel);
            if (prime * prime > num) {
                if (stream) fprintf(stream, "%lu\n", num);
                count++;
                break;
            }
            if (unique) {
                if (num % prime == 0) {
                    if (stream) fprintf(stream, "%lu\n", prime);
                    count++;
                    for (num /= prime; num % prime == 0; num /= prime);
                }
            } else {
                while (num % prime == 0) {
                    if (stream) fprintf(stream, "%lu\n", prime);
                    num /= prime;
                    count++;
                }
            }
        }

        /* Deallocate memory associated with the wheel */
        deleteWheel(&wheel);
    }

    return count;
}

int main(int argc, const char **argv) {
    if (argc == 1) {
        /* If there are no command line arguments, print usage message */
        fprintf(stderr, "divisors: expected argument.\n");
        fprintf(stderr, "Usage: to list the prime divisors of N:\n");
        fprintf(stderr, "\t%s N\n", argv[0]);
        /* Exit with failure */
        return 1;
    }

    while (*(++argv)) {
        factor(strtoul(*argv, NULL, 10), 0, stdout);
    }
    return 0;
}
