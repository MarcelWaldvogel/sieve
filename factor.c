/*******************************************************************************
FILE:           factor.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-25
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

void helpmessage(const char *);

int main(int argc, const char **argv) {
    const char *name = argv[0];     /* The program name */
    unsigned char opt_count = 0;    /* Flag to print the number of factors */
    unsigned char opt_unique = 0;   /* Flag to ignore multiplicity */
    unsigned long num;              /* The number to be factored */
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
                case 'u':
                    opt_unique = 1;
                    break;
                default:
                    fprintf(stderr, "factors: illegal option '%c'\n", c);
                    return 1;
            }
        }
    }

    /* There should be one command-line argument remaining */
    /* If not, print a usage message and exit with failure */
    if (argc != 1) {
        fprintf(stderr, "factors: expected argument.\n");
        fprintf(stderr, "For help, run %s -h\n", name);
        return 1;
    }

    /* Convert the number specified by the user to an unsigned long */
    num = strtoul(*argv, NULL, 10);

    if (opt_count)
        printf("%lu\n", factor(num, opt_unique, NULL));
    else
        factor(num, opt_unique, stdout);

    return 0;
}

void helpmessage(const char *name) {
    fprintf(stderr, "Print the prime factors of a positive integer.\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "\t%s N\n", name);
    fprintf(stderr, "\t%s [flags] N\n", name);
    fprintf(stderr, "\twhere N is a positive integer.\n");
    fprintf(stderr, "Flags:\n");
    fprintf(stderr, "\t-n\tPrint only the number of factors.\n");
    fprintf(stderr, "\t-u\tIgnore multiplicity.\n");
    fprintf(stderr, "\t-h\tShow usage information.\n");
}
