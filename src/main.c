/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-07-20
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>

#include "sieve.h"
#include "main.h"

/*
 * STRUCT:      Options
 * DESCRIPTION: Organizes all the command-line options for the program.
 */
typedef struct {
    unsigned int count;     /* Print only the number of primes */
    unsigned int help;      /* Print help message */
    unsigned int input;     /* Read argument from stdin */
} Options;

/* Static ("private") function prototypes */
static void process_options(int *, const char ***, Options *);
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
    Options ops;                /* Command-line options */

    /* Set up interrupt handling */
    signal(SIGINT, &interrupt);

    /* Parse command-line options and set the global option flags */
    process_options(&argc, &argv, &ops);

    /* Print help message if necessary, then exit */
    if (ops.help) {
        printf(HELP_MESSAGE, OPT_COUNT, OPT_STDIN);
        return EXIT_SUCCESS;
    }

    /* Check whether to get program argument from stdin or the command-line */
    if ((argc > NUM_ARGS) || (argc && ops.input)) {
        /* There are too many arguments -- show error and exit */
        sieve_error(ERR_TOO_MANY_ARGS);
    } else if (argc < NUM_ARGS) {
        /* There are no command-line arguments. Check first if we should be
         * reading from stdin */
        if(ops.input) {
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

    /* Sieve the primes up to the specified number */
    if (ops.count) {
        /* Print only the number of primes up to num */
        printf(COUNT_FMT, sieve(num, NULL));
    } else {
        /* Print all the primes up to num */
        sieve(num, stdout);
    }

    return EXIT_SUCCESS;
}


/*
 * FUNCTION:    process_options
 * DESCRIPTION: Process command-line options for the program.
 * PARAMETERS:  argcp (int *): Pointer to the number of command-line arguments
 *              argvp (const char ***): Pointer to the command-line arguments
 *              ops (Options *): Pointer to a container to store options
 */
static void process_options(int *argcp, const char ***argvp, Options *ops) {
    int c; /* Command-line argument character */

    opterr = 0; /* Reset global option-handling error code */

    /* Initialize all options */
    ops->count = 0;
    ops->help = 0;
    ops->input = 0;

    /* Iterate over all options found by getopt */
    while ((c = getopt(*argcp, (char * const *) *argvp, ALL_OPTS)) != -1) {
        /* Process the option character */
        switch (c) {
            case OPT_HELP:
                ops->help = 1;
                break;
            case OPT_COUNT:
                ops->count = 1;
                break;
            case OPT_STDIN:
                ops->input = 1;
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
    fprintf(stderr, PROGRAM_NAME ": ");
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, ERR_USAGE_HELP, OPT_HELP);
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
