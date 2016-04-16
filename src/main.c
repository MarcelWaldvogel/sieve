/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-15
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include "sieve.h"
#include "factor.h"
#include "main.h"

/* Command-line option processing results */
#define OP_FAILURE  0
#define OP_SUCCESS  1

/* Static ("private") function prototypes */
static void process_options(int *, const char ***); /* Parse program options */
static void sieve_error(const char *, ...);         /* Print error and exit */

/* Global variables */
static const char *prog_name;   /* Name of the program */
static int opt_factor = 0;      /* Option: factor number instead of sieving */
static int opt_count = 0;       /* Option: print only the number of primes */
static int opt_unique = 0;      /* Option: ignore divisor multiplicity */
static int opt_help = 0;        /* Option: print help message */
static int opt_stdin = 0;       /* Option: read argument from stdin */


/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    unsigned long num;          /* Sieve upper bound OR number to factor */
    char *endptr;               /* For stroul's error checking */
    char *nlpos;                /* Position of first newline in the argument */
    char str[BUFSIZ];           /* String to be used as the program argument */

    /* Get the name of the program */
    prog_name = *argv;

    /* Parse command-line options and set the global option flags */
    process_options(&argc, &argv);

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor)
        sieve_error(ERR_U_WITHOUT_F);

    /* Print help message if necessary, then exit */
    if (opt_help) {
        printf(HELP_MESSAGE, prog_name,
                OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_STDIN);
        return EXIT_SUCCESS;
    }

    /* Check whether to get program argument from stdin or the command-line */
    if ((argc > NUM_ARGS) || (argc && opt_stdin)) {
        /* There are too many arguments -- show error and exit */
        sieve_error(ERR_TOO_MANY_ARGS);
    } else if (argc < NUM_ARGS) {
        /* There are no command-line arguments. Check first if we should be
         * reading from stdin */
        if(opt_stdin) {
            /* Try reading from stdin */
            if (!fgets(str, BUFSIZ, stdin))
                /* If fgets fails, print error and exit */
                sieve_error(ERR_READ_STDIN);
        } else {
            /* Not reading from stdin and no command-line arg */
            sieve_error(ERR_EXPECTED_ARG);
        }
    } else {
        /* Otherwise, there is one command-line argument left--copy it to str */
        (void) strncpy(str, *argv, BUFSIZ);
        if (str[BUFSIZ - 1] != '\0')
            /* The argument string was not null-terminated because it is too
             * long. Print error and exit */
            sieve_error(ERR_TOO_LONG);
    }

    /* Strip potential trailing newline */
    if ((nlpos = strchr(str, '\n')))
        *nlpos = '\0';

    /* Look for a minus sign in the argument (this isn't done by strtoul) */
    if (strchr(str, MINUS))
        sieve_error(ERR_CONVERT, str);

    /* Convert the command-line number to an unsigned long */
    num = strtoul(str, &endptr, BASE);

    /* Check if the argument was successfully converted */
    if (endptr == str || *endptr)
        sieve_error(ERR_CONVERT, str);
    else if (errno == ERANGE)
        sieve_error(ERR_TOO_LARGE, str);

    /* Perform either sieving or factoring */
    if (!opt_factor) {
        /* Sieve the primes up to the specified number */
        if (opt_count)
            /* Print only the number of primes up to num */
            printf(COUNT_FMT, sieve(num, NULL));
        else
            /* Print all the primes up to num */
            sieve(num, stdout);
    } else {
        /* Factor the specified number */
        if (opt_count)
            /* Print the number of factors (with or without multiplicity) */
            printf(COUNT_FMT, factor(num, opt_unique, NULL));
        else
            /* Print all the prime factors (with or without multiplicity) */
            factor(num, opt_unique, stdout);
    }

    return EXIT_SUCCESS;
}


/*
 * FUNCTION:    process_options
 * DESCRIPTION: Process command-line options for the program.
 * PARAMETERS:  argcp (int *): Pointer to the number of command-line arguments
 *              argvp (const char ***): Pointer to the command-line arguments
 */
static void process_options(int *argcp, const char ***argvp) {
    int c; /* Command-line argument character */

    opterr = 0; /* Reset global option-handling error code */

    /* Iterate over all options found by getopt */
    while ((c = getopt(*argcp, (char * const *) *argvp, ALL_OPTS)) != -1) {
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
            case OPT_STDIN:
                opt_stdin = 1;
                break;
            default:
                sieve_error(ERR_ILLEGAL_OPTION, optopt);
        }
    }

    *argcp -= optind;   /* Update number of non-option arguments */
    *argvp += optind;   /* Have main's argv point to first non-option */
}


/*
 * FUNCTION:    sieve_error
 * DESCRIPTION: Print a specialized error message followed by a generic help
 *              message.
 */
static void sieve_error(const char *format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, ERR_USAGE_HELP, prog_name, OPT_HELP);
    exit(EXIT_FAILURE);
}
