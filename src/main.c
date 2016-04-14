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

/* Static function prototypes */
static int process_options(void);           /* Process command-line options */
static int sieve_error(const char *, ...);  /* Print error message */
static void help_message(void);             /* Print help if using option -h */

/* Global variables */
static int num_arguments;       /* Number of args after option processing */
static const char **arguments;  /* Command-line arguments left after options */
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
    int op_result;              /* Result from processing options */
    char *endptr;               /* For stroul's error checking */
    char str[BUFSIZ];           /* String to be used as the program argument */
    int i;                      /* Loop index */

    /* Get the name of the program and the command-line arguments */
    prog_name = *argv;
    arguments = argv;
    num_arguments = argc;

    /* Process command-line options */
    op_result = process_options();
    if (op_result == OP_FAILURE)
        return EXIT_FAILURE;

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor)
        return sieve_error(ERR_U_WITHOUT_F);

    /* Print help message if necessary, then exit */
    if (opt_help) {
        help_message();
        return EXIT_SUCCESS;
    }

    /* Check whether to get program argument from stdin or the command-line */
    if ((num_arguments > NUM_ARGS) || (num_arguments && opt_stdin)) {
        /* There are too many arguments -- show error and exit */
        return sieve_error(ERR_TOO_MANY_ARGS);
    } else if (num_arguments < NUM_ARGS) {
        /* There are no command-line arguments */
        if(opt_stdin) {
            /* Try reading from stdin */
            if (!fgets(str, BUFSIZ, stdin))
                /* If fgets fails, print error and exit */
                return sieve_error(ERR_READ_STDIN);
        } else {
            /* Not reading from stdin and no command-line arg */
            return sieve_error(ERR_EXPECTED_ARG);
        }
    } else {
        /* Otherwise, there is one command-line argument left--copy it to str */
        (void) strncpy(str, *arguments, BUFSIZ);
        if (str[BUFSIZ - 1] != '\0')
            /* The argument string was not null-terminated because it is too
             * long. Print error and exit */
            return sieve_error(ERR_TOO_LONG);
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
        return sieve_error(ERR_CONVERT, str);

    /* Convert the command-line number to an unsigned long */
    num = strtoul(str, &endptr, BASE);

    /* Check if the argument was successfully converted */
    if (endptr == str || *endptr)
        return sieve_error(ERR_CONVERT, str);
    else if (errno == ERANGE)
        return sieve_error(ERR_TOO_LARGE, str);

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
static int process_options(void) {
    char c; /* Command-line argument character */

    while (--num_arguments > 0 && **++arguments == OPT_START) {
        c = *++*arguments;   /* Read next character following OPT_START */
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
                    ++arguments;
                    --num_arguments;
                    return OP_SUCCESS;
                case OPT_NULL:
                    (void) sieve_error(ERR_EXPECTED_OPT, OPT_START);
                    return OP_FAILURE;
                default:
                    (void) sieve_error(ERR_ILLEGAL_OPTION, c);
                    return OP_FAILURE;
            }
        } while ((c = *++*arguments));
    }

    return OP_SUCCESS;
}


/*
 * FUNCTION:    sieve_error
 * DESCRIPTION: Print a specialized error message followed by a generic help
 *              message.
 */
static int sieve_error(const char *format, ...) {
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    fprintf(stderr, ERR_USAGE_HELP, prog_name, OPT_HELP);
    return EXIT_FAILURE;
}


/*
 * FUNCTION:    help_message
 * DESCRIPTION: Print a message to the user about the usage of this program.
 */
static void help_message(void) {
    printf(HELP_MESSAGE, prog_name,
            OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_STDIN, OPT_END);
}
