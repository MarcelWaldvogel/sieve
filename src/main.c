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
#include <getopt.h>
#include <signal.h>

#include "sieve.h"
#include "main.h"

/* Static ("private") function prototypes */
static void process_options(int *, const char ***);
static void sieve_error(const char *, ...);
static void interrupt(int);

/* Store the command-line options */
static struct {
    int count;  /* If 1, count the number of primes instead of listing them */
    int help;   /* If 1, display help message and exit */
    int input;  /* If 1, read argument from stdin */
} options;

/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    unsigned long num;          /* Sieve upper bound */
    char *endptr;               /* For stroul's error checking */
    char *nlpos;                /* Position of first newline in the argument */
    char str[BUFSIZ];           /* String to be used as the program argument */

    /* Set up interrupt handling */
    signal(SIGINT, &interrupt);

    /* Process the command-line options */
    process_options(&argc, &argv);

    /* Print help message and exit if necessary */
    if (options.help) {
        printf(HELP_MESSAGE, OP_COUNT, OP_STDIN);
        return EXIT_SUCCESS;
    }

    /* Check whether to get program argument from stdin or the command-line */
    if ((argc > NUM_ARGS) || (argc && options.input)) {
        /* There are too many arguments -- show error and exit */
        sieve_error(ERR_TOO_MANY_ARGS);
    } else if (argc < NUM_ARGS) {
        /* There are no command-line arguments. Check first if we should be
         * reading from stdin */
        if(options.input) {
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
    if (options.count) {
        /* Print only the number of primes up to num */
        printf(COUNT_FMT, sieve_count(num));
    } else {
        /* Print all the primes up to num */
        sieve_list(num);
    }

    return EXIT_SUCCESS;
}


/*
 * FUNCTION:    process_options
 * DESCRIPTION: Iterate over the command-line options (the ones prefixed with
 *              '-' and set the values of the global options struct.
 * PARAMETERS:  argcp (int *): Pointer to argc in main.
                argvp (const char ***): Pointer to argv in main.
 */
static void process_options(int *argcp, const char ***argvp) {
    int c;      /* Command-line argument character */
    opterr = 0; /* Reset global option-handling error code */

    /* Set default command-line option values */
    options.count = 0;
    options.help = 0;
    options.input = 0;

    /* Iterate over all options found by getopt */
    while ((c = getopt(*argcp, (char * const *) *argvp, ALL_OPS)) != -1) {
        /* Process the option character */
        switch (c) {
            case OP_HELP:
                options.help = 1;
                break;
            case OP_COUNT:
                options.count = 1;
                break;
            case OP_STDIN:
                options.input = 1;
                break;
            default:
                sieve_error(ERR_ILLEGAL_OPTION, optopt);
        }
    }

    *argcp -= optind;   /* Update number of non-option arguments */
    *argvp += optind;   /* Make argv point to first non-option */
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
