/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <signal.h>

#include "sieve.h"
#include "main.h"

/* Static ("private") function prototypes */
static void sieve_error(const char *, ...);
static void interrupt(int);

/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    unsigned long num;          /* Sieve upper bound */
    char *endptr;               /* For stroul's error checking */
    char *nlpos;                /* Position of first newline in the argument */
    char str[BUFSIZ];           /* String to be used as the program argument */
    int op_count = 0;           /* Option to count the number of primes */
    int op_help = 0;            /* Option to display help message */
    int op_stdin = 0;           /* Option to read argument from stdin */
    int c;                      /* Command-line argument character */

    /* Set up interrupt handling */
    signal(SIGINT, &interrupt);

    /*
     * Parse command-line options
     */
    opterr = 0; /* Reset global option-handling error code */

    /* Iterate over all options found by getopt */
    while ((c = getopt(argc, (char * const *) argv, ALL_OPS)) != -1) {
        /* Process the option character */
        switch (c) {
            case OP_HELP:
                op_help = 1;
                break;
            case OP_COUNT:
                op_count = 1;
                break;
            case OP_STDIN:
                op_stdin = 1;
                break;
            default:
                sieve_error(ERR_ILLEGAL_OPTION, optopt);
        }
    }

    argc -= optind;     /* Update number of non-option arguments */
    argv += optind;     /* Make argv point to first non-option */

    /* Print help message and exit if necessary */
    if (op_help) {
        printf(HELP_MESSAGE, OP_COUNT, OP_STDIN);
        return EXIT_SUCCESS;
    }

    /*
     * Get the command-line argument
     */
    /* Check whether to get program argument from stdin or the command-line */
    if ((argc > NUM_ARGS) || (argc && op_stdin)) {
        /* There are too many arguments -- show error and exit */
        sieve_error(ERR_TOO_MANY_ARGS);
    } else if (argc < NUM_ARGS) {
        /* There are no command-line arguments. Check first if we should be
         * reading from stdin */
        if(op_stdin) {
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

    /*
     * Perform the sieving
     */
    if (op_count) {
        /* Print only the number of primes up to num */
        printf(COUNT_FMT, sieve_count(num));
    } else {
        /* Print all the primes up to num */
        sieve_list(num);
    }

    return EXIT_SUCCESS;
}


/*
 * FUNCTION:    sieve_error
 * DESCRIPTION: Print a specialized error message followed by a generic help
 *              message.
 */
static void sieve_error(const char *format, ...) {
    va_list argptr;
    va_start(argptr, format);
    fprintf(stderr, PROGRAM_NAME ": ");
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, ERR_USAGE_HELP, OP_HELP);
    exit(EXIT_FAILURE);
}


/*
 * FUNCTION:    interrupt
 * DESCRIPTION: Print a message indicating that the program was interrupted.
 * PARAMETERS:  sig (int): Unused. Needed to make function match the prototype
 *              required by the `signal' method.
 */
static void interrupt(int sig) {
    (void) sig; /* Ignore the parameter */
    sieve_error(ERR_INTERRUPT);
}
