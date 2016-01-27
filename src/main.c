/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-01-27
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include "sieve.h"
#include "factor.h"
#include "main_strings.h"

/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    const char *name = *argv;   /* The program name */
    unsigned long num;          /* Sieve upper bound OR number to factor */
    int opt_factor = 0;         /* Option: factor number instead of sieving */
    int opt_count = 0;          /* Option: print only the number of primes */
    int opt_unique = 0;         /* Option: ignore divisor multiplicity */
    char c;                     /* Command-line argument character */
    char *endptr;               /* For stroul's error checking */

    /* Process command-line options */
    while (--argc > 0 && **++argv == '-') {
        while ((c = *++*argv)) {
            switch (c) {
                case 'h':
                    printf(HELP_MESSAGE, name);
                    return EXIT_SUCCESS;
                case 'f':
                    opt_factor = !opt_factor;
                    break;
                case 'n':
                    opt_count = !opt_count;
                    break;
                case 'u':
                    opt_unique = !opt_unique;
                    break;
                default:
                    fprintf(stderr, ERR_ILLEGAL_OPTION, c);
                    fprintf(stderr, ERR_USAGE_HELP, name);
                    return EXIT_FAILURE;
            }
        }
    }

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor) {
        fprintf(stderr, ERR_U_WITHOUT_F);
        fprintf(stderr, ERR_USAGE_HELP, name);
        return EXIT_FAILURE;
    }

    /*
     * There should be one command-line argument remaining. If not, print a
     * usage message and exit with failure.
     */
    if (argc < 1) {
        fprintf(stderr, ERR_EXPECTED_ARG);
        fprintf(stderr, ERR_USAGE_HELP, name);
        return EXIT_FAILURE;
    } else if (argc > 1) {
        fprintf(stderr, ERR_TOO_MANY_ARGS);
        fprintf(stderr, ERR_USAGE_HELP, name);
        return EXIT_FAILURE;
    }

    /* Convert the command-line number to an unsigned long */
    num = strtoul(*argv, &endptr, 10);

    /* Check if the argument was successfully converted */
    if (*endptr) {
        fprintf(stderr, ERR_CONVERT, *argv);
        return EXIT_FAILURE;
    }

    if (!opt_factor) {
        /* Sieve the primes up to the specified number */
        if (opt_count)
            printf("%lu\n", sieve(num, NULL));
        else
            sieve(num, stdout);
    } else {
        /* Factor the specified number */
        if (opt_count)
            printf("%lu\n", factor(num, opt_unique, NULL));
        else
            factor(num, opt_unique, stdout);
    }

    return EXIT_SUCCESS;
}
