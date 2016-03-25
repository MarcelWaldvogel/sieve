/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-03-25
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "sieve.h"
#include "factor.h"
#include "main.h"

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
    int opt_help = 0;           /* Option: print help message */
    char c;                     /* Command-line argument character */
    char *endptr;               /* For stroul's error checking */
    const char *str;            /* String used to check if argument has a '-' */

    /* Process command-line options */
    while (--argc > 0 && **++argv == OPT_START) {
        c = *++*argv;   /* Read next character following OPT_START */
        do {
            switch (c) {
                case OPT_HELP:
                    opt_help = 1;
                    break;
                case OPT_FACTOR:
                    opt_factor = 1;
                    break;
                case OPT_COUNT:
                    opt_count = 1;
                    break;
                case OPT_UNIQUE:
                    opt_unique = 1;
                    break;
                case OPT_END:
                    argv++;
                    argc--;
                    goto post_options;
                case OPT_NULL:
                    fprintf(stderr, ERR_EXPECTED_OPT, OPT_START);
                    fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
                    return EXIT_FAILURE;
                default:
                    fprintf(stderr, ERR_ILLEGAL_OPTION, c);
                    fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
                    return EXIT_FAILURE;
            }
        } while ((c = *++*argv));
    }

post_options:

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor) {
        fprintf(stderr, ERR_U_WITHOUT_F);
        fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
        return EXIT_FAILURE;
    }

    /* Print help message if necessary */
    if (opt_help) {
        printf(HELP_MESSAGE, name, OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_END);
        return EXIT_SUCCESS;
    }

    /* There should be one command-line argument remaining */
    if (argc < NUM_ARGS) {
        fprintf(stderr, ERR_EXPECTED_ARG);
        fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
        return EXIT_FAILURE;
    } else if (argc > NUM_ARGS) {
        fprintf(stderr, ERR_TOO_MANY_ARGS);
        fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
        return EXIT_FAILURE;
    }

    /* Look for a minus sign in the command-line argument (apparently this isn't
     * done by strtoul) */
    for (str = *argv; *str; str++) {
        if (*str == MINUS) {
            fprintf(stderr, ERR_CONVERT, *argv);
            fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
            return EXIT_FAILURE;
        }
    }

    /* Convert the command-line number to an unsigned long */
    num = strtoul(*argv, &endptr, BASE);

    /* Check if the argument was successfully converted */
    if (endptr == *argv || *endptr) {
        fprintf(stderr, ERR_CONVERT, *argv);
        fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
        return EXIT_FAILURE;
    } else if (errno == ERANGE) {
        fprintf(stderr, ERR_TOO_LARGE, *argv);
        fprintf(stderr, ERR_USAGE_HELP, name, OPT_HELP);
        return EXIT_FAILURE;
    }

    if (!opt_factor) {
        /* Sieve the primes up to the specified number */
        if (opt_count)
            printf(COUNT_FMT, sieve(num, NULL));
        else
            sieve(num, stdout);
    } else {
        /* Factor the specified number */
        if (opt_count)
            printf(COUNT_FMT, factor(num, opt_unique, NULL));
        else
            factor(num, opt_unique, stdout);
    }

    return EXIT_SUCCESS;
}
