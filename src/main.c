/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-01-10
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include "sieve.h"
#include "factor.h"

#define ERR_ILLEGAL_OPTION  "sieve: illegal option `%c'\n"
#define ERR_EXPECTED_ARG    "sieve: expected argument.\n"
#define ERR_TOO_MANY_ARGS   "sieve: too many arguments.\n"
#define ERR_U_WITHOUT_F     "sieve: the -u option cannot be used without -f\n"
#define ERR_USAGE_HELP      "For help, run %s -h\n"

#define HELP_PROG_NAME      "WheelSieve"
#define HELP_DESCRIPTION    "\tSieve of Eratosthenes and prime factorization\n"
#define HELP_USAGE          "Usage:\n\t%s [-f] [-n] [-u] <positive integer>\n"
#define HELP_OPTIONS        "Options:\n"
#define HELP_OPT_N          "\t-n\tShow only the number of primes\n"
#define HELP_OPT_F          "\t-f\tFactor the given integer. Off by default\n"
#define HELP_OPT_U          "\t-u\tWhen using -f option, ignore multiplicity\n"

/*
 * FUNCTION:    print_help
 * DESCRIPTION: Print a help message for the user
 */
void print_help(const char *name) {
    fprintf(stdout, HELP_PROG_NAME);
    fprintf(stdout, HELP_DESCRIPTION);
    fprintf(stdout, HELP_USAGE, name);
    fprintf(stdout, HELP_OPTIONS);
    fprintf(stdout, HELP_OPT_N);
    fprintf(stdout, HELP_OPT_F);
    fprintf(stdout, HELP_OPT_U);
}

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
    char c;			            /* Command-line argument character */

    /* Process command-line options */
    while (--argc > 0 && **++argv == '-') {
        while ((c = *++*argv)) {
            switch (c) {
                case 'h':
                    print_help(name);
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
    num = strtoul(*argv, NULL, 10);

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
