/*******************************************************************************
FILE:           divisors.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-18
DESCRIPTION:    Prints out the prime divisors of a given positive integer.
                Prime divisors are computed using trial division with wheel
                factorization.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wheel.h"

const unsigned long basePrimes[] = {2, 3, 5, 7, 11, 13};
const unsigned long numBasePrimes = 6;

void print_divisors(unsigned long num) {
    if (num > 1) {
        unsigned long prime;
        /* Divide by the base primes */
        for (unsigned long index = 0; index < numBasePrimes; index++) {
            prime = basePrimes[index];
            while (num % prime == 0) {
                printf("%lu\n", prime);
                num /= prime;
            }
        }

        /* Create the wheel for wheel factorization */
        Wheel *wheel = newWheel(basePrimes, numBasePrimes);
        nextp(wheel); /* Ignore 1 */
        /* Divide by the prime candiates computed by the wheel */
        while (num > 1) {
            prime = nextp(wheel);
            if (prime * prime > num) {
                printf("%lu\n", num);
                break;
            }
            while (num % prime == 0) {
                printf("%lu\n", prime);
                num /= prime;
            }
        }
        /* Deallocate the memory associated with the wheel */
        deleteWheel(&wheel);
    }
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
        print_divisors(strtoul(*argv, NULL, 10));
    }
}
