#include <stdio.h>
#include <stdlib.h>
#include "../wheel.h"

#define MAX 20 /* Number of prime number candidates to print */

int main(int argc, const char **argv) {
    int i;
    Wheel *wheel;
    unsigned long numBasePrimes;
    unsigned long *basePrimes;

    if (argc == 1) {
        /* If there are no command-line arguments, print usage message */
        fprintf(stderr,
                "Usage: Enter a list of primes: e.g.,\t\n%s 2 3 5\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Allocate memory for the base primes */
    numBasePrimes = (unsigned long) argc - 1;
    basePrimes = malloc(numBasePrimes * sizeof(unsigned long));

    /* Print the base primes */
    printf("The %lu base primes:\n", numBasePrimes);
    for (i = 1; i < argc; i++) {
        /* Parse command line arguments as primes (no error checking) */
        basePrimes[i - 1] = strtoul(argv[i], NULL, 0);
        printf("Base prime      #%i:\t%3ld\n", i, basePrimes[i - 1]);
    }

    /* Create the wheel to be tested */
    wheel = newWheel(basePrimes, numBasePrimes);

    /* Print the prime candidates */
    printf("The first %i prime candidates:\n", MAX);
    for (i = 1; i <= MAX; i++)
        printf("Prime candidate #%i\t%3ld\n", i, nextp(wheel));

    /* Clean up and return */
    free(basePrimes);
    deleteWheel(&wheel);
    return EXIT_SUCCESS;
}
