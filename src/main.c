/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-23
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

#ifdef DEBUG_ON
#include "debug.h"
#endif

/*
 * STRUCT:      Options
 * DESCRIPTION: Organizes all the command-line options for the program.
 */
typedef struct {
    unsigned int count;     /* Print only the number of primes */
    unsigned int factor;    /* Factor number instead of sieveing */
    unsigned int help;      /* Print help message */
    unsigned int input;     /* Read argument from stdin */
    unsigned int unique;    /* Ignore divisor multiplicity */
} Options;

/* Static ("private") function prototypes */
static void process_options(int *, const char ***, Options *);
static void sieve_error(const char *, ...);

/* Global variables */
static const char *prog_name;   /* Name of the program */


/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    unsigned long num;          /* Sieve upper bound OR number to factor */
    char *endptr;               /* For stroul's error checking */
    char *nlpos;                /* Position of first newline in the argument */
    char str[BUFSIZ];           /* String to be used as the program argument */
    Options ops;                /* Command-line options */

    /* Get the name of the program */
    prog_name = *argv;

    /* Parse command-line options and set the global option flags */
    process_options(&argc, &argv, &ops);

    /* The -u option shouldn't be used without the -f option */
    if (ops.unique && !ops.factor)
        sieve_error(ERR_U_WITHOUT_F);

    /* Print help message if necessary, then exit */
    if (ops.help) {
        printf(HELP_MESSAGE, prog_name,
                OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_STDIN);
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

    /* Debugging: print the final argument string */
    #ifdef DEBUG_ON
        DEBUG_MSG("Argument string: `%s'", str);
    #endif

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

    /* Debugging: print the converted number */
    #ifdef DEBUG_ON
        DEBUG_MSG("Converted number: %lu", num);
    #endif

    /* Perform either sieving or factoring */
    if (!ops.factor) {
        /* Sieve the primes up to the specified number */
        if (ops.count)
            /* Print only the number of primes up to num */
            printf(COUNT_FMT, sieve(num, NULL));
        else
            /* Print all the primes up to num */
            sieve(num, stdout);
    } else {
        /* Factor the specified number */
        if (ops.count)
            /* Print the number of factors (with or without multiplicity) */
            printf(COUNT_FMT, factor(num, ops.unique, NULL));
        else
            /* Print all the prime factors (with or without multiplicity) */
            factor(num, ops.unique, stdout);
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
    ops->factor = 0;
    ops->help = 0;
    ops->input = 0;
    ops->unique = 0;

    /* Iterate over all options found by getopt */
    while ((c = getopt(*argcp, (char * const *) *argvp, ALL_OPTS)) != -1) {
        /* Debugging: print current option character being processed */
        #ifdef DEBUG_ON
            DEBUG_MSG("Processing option `%c'", (c != '?') ? c : optopt);
        #endif

        /* Process the option character */
        switch (c) {
            case OPT_HELP:
                ops->help = 1;
                break;
            case OPT_FACTOR:
                ops->factor = 1;
                break;
            case OPT_COUNT:
                ops->count = 1;
                break;
            case OPT_UNIQUE:
                ops->unique = 1;
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
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, ERR_USAGE_HELP, prog_name, OPT_HELP);
    exit(EXIT_FAILURE);
}
