/*******************************************************************************
FILE:           sieve.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-19
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

unsigned long sieve(unsigned long max, FILE *stream) {
    /* Step 0 -- Local variable declarations */
    unsigned char *isPrime; /* Array of T/F values */
    Wheel *wheel;           /* The wheel used in the sieve */
    unsigned long prime;    /* A prime candidate */
    unsigned long comp;     /* A necessarily composite number */
    unsigned long count;    /* The number of primes */
    unsigned long index;    /* Track position in loops */

    /* Step 1 -- Create the sieving array */
    isPrime = (unsigned char *) malloc((max + 1) * sizeof(unsigned char));
    memset(isPrime, TRUE, (max + 1) * sizeof(unsigned char));
    isPrime[0] = isPrime[1] = FALSE;

    /* Step 2 -- Sieve out the base primes */
    count = 0;
    for (index = 0; index < numBasePrimes; index++) {
        prime = basePrimes[index];
        if (prime > max)
            break;
        count++;
        fprintf(stream, "%lu\n", prime);
        for (comp = prime * prime; comp <= max; comp += prime)
            isPrime[comp] = FALSE;
    }

    /* Step 3 -- Create the wheel for wheel factorization */
    wheel = newWheel(basePrimes, numBasePrimes);
    nextp(wheel); /* Ignore 1 */

    /* Step 4 -- Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (isPrime[prime]) {
            count++;
            fprintf(stream, "%lu\n", prime);
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                isPrime[comp] = FALSE;
        }
    }

    /* Step 5 -- Sieve the remaining primes > sqrt(max) */
    for (; prime <= max; prime = nextp(wheel)) {
        if (isPrime[prime]) {
            count++;
            fprintf(stream, "%lu\n", prime);
        }
    }

    /* Step 6 -- Clean up and return */
    free(isPrime);
    deleteWheel(&wheel);
    return count;
}

int main(int argc, const char **argv) {
    const char *name = argv[0];         /* The program name */
    unsigned char opt_count = FALSE;    /* The count flag */
    unsigned long count;                /* The number of primes */
    FILE *stream = stdout;              /* Where to print the primes */
    char c;                             /* A command line argument char */

    /* Process command-line options */
    while (--argc > 0 && **++argv == '-') {
        while ((c = *++*argv)) {
            switch (c) {
                case 'h':
                    fprintf(stderr, "Print all the primes up to a given ");
                    fprintf(stderr, "positive integer.\n");
                    fprintf(stderr, "Usage:\n");
                    fprintf(stderr, "\t%s N\n", name);
                    fprintf(stderr, "\t%s [flags] N\n", name);
                    fprintf(stderr, "\twhere N is a positive integer.\n");
                    fprintf(stderr, "Flags:\n");
                    fprintf(stderr, "\t-n\tPrint only the number of primes.\n");
                    fprintf(stderr, "\t-h\tShow usage information.\n");
                    return 0;
                case 'n':
                    opt_count = TRUE;
                    stream = fopen("/dev/null", "w");
                    if (!stream) {
                        fprintf(stderr,
                            "Could not open /dev/null for writing\n");
                        return 1;
                    }
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
        fclose(stream);
        return 1;
    }

    /* Perform the sieving */
    count = sieve(strtoul(*argv, NULL, 10), stream);

    /* Print the number of primes if the count flag is on */
    if (opt_count) printf("%lu\n", count);

    /* Clean up and return */
    fclose(stream);
    return 0;
}
