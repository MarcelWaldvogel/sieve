/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-14
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "sieve.h"
#include "factor.h"
#include "main.h"

/* Command-line option processing results */
#define OP_FAILURE  0
#define OP_SUCCESS  1

/* Static ("private") function prototypes */
static void process_options(int *, const char ***);
static void sieve_error(const char *, ...);
static void help_message(void);

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
    char str[BUFSIZ];           /* String to be used as the program argument */
    int i;                      /* Loop index */

    /* Get the name of the program */
    prog_name = *argv;

    /* Process command-line options */
    process_options(&argc, &argv);

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor)
        sieve_error(ERR_U_WITHOUT_F);

    /* Print help message if necessary, then exit */
    if (opt_help) {
        help_message();
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
    for (i = 0; i < BUFSIZ && str[i]; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }

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


/*
 * FUNCTION:    process_options
 * DESCRIPTION: Process command-line options for the program
 */
static void process_options(int *argcp, const char ***argvp) {
    char c; /* Command-line argument character */

    /* Loop while there are still command line arguments beginning with '-' */
    while (--*argcp > 0 && **++*argvp == OPT_START) {
        c = *++**argvp;   /* Read next character following OPT_START */
        /* Process all options in the current command-line argument */
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
                case OPT_STDIN:
                    opt_stdin = 1;
                    break;
                case OPT_END:
                    ++*argvp;
                    --*argcp;
                    return;
                case OPT_NULL:
                    sieve_error(ERR_EXPECTED_OPT, OPT_START);
                default:
                    sieve_error(ERR_ILLEGAL_OPTION, c);
            }
        } while ((c = *++**argvp));
    }
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


/*
 * FUNCTION:    help_message
 * DESCRIPTION: Print a message to the user about the usage of this program.
 */
static void help_message(void) {
    printf(HELP_MESSAGE, prog_name,
            OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_STDIN, OPT_END);
}
