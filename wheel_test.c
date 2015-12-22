/*******************************************************************************
FILE:           wheel_test.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-21
DESCRIPTION:    Tests the wheel implementation.
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "wheel.h"

#define MAX 20 /* Number of prime candidates to print */

int main(const int argc, const char **argv) {
    if (argc == 1) {
        /* If there are no command line arguments, print usage message */
        fprintf(stderr,
            "Usage: Enter a list of primes: e.g.\n%s 2 3 5\n", argv[0]);
        /* Exit with failure */
        return 1;
    }

    /* Allocate memory for the base primes */
    unsigned long numBasePrimes = (unsigned long) argc - 1;
    unsigned long *basePrimes
        = (unsigned long *) malloc(numBasePrimes * sizeof(unsigned long));

    printf("The %lu base primes:\n", numBasePrimes);

    for (int i = 1; i < argc; i++) {
        /* Parse command line args as unsigned longs (no error checking!!) */
        basePrimes[i - 1] = strtoul(argv[i], NULL, 0);
        printf("Base prime      #%i:\t%3ld\n", i, basePrimes[i - 1]);
    }

    /* Create the wheel to be tested */
    Wheel *wheel = newWheel(basePrimes, numBasePrimes);

    /* Print the prime candidates */
    printf("The first %i prime candidates:\n", MAX);
    for (int i = 1; i <= MAX; i++) {
        unsigned long p = nextp(wheel);
        printf("Prime candidate #%i:\t%3ld\n", i, p);
    }

    /* Clean up and return */
    free(basePrimes);
    deleteWheel(&wheel);
    return 0;
}
