/*
 * FILE:        main.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-03-27
 * DESCRIPTION: Contains the driver for the sieve program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <limits.h>
#include "sieve.h"
#include "factor.h"
#include "main.h"


/* Print error message */
static int sieve_error(const char *, ...);

/* Name of the program */
static const char *prog_name;

/*
 * FUNCTION:    main
 * DESCRIPTION: Driver for the sieve program.
 */
int main(int argc, const char **argv) {
    unsigned long num;          /* Sieve upper bound OR number to factor */
    int opt_factor = 0;         /* Option: factor number instead of sieving */
    int opt_count = 0;          /* Option: print only the number of primes */
    int opt_unique = 0;         /* Option: ignore divisor multiplicity */
    int opt_help = 0;           /* Option: print help message */
    int opt_stdin = 0;          /* Option: read argument from stdin */
    char c;                     /* Command-line argument character */
    char *endptr;               /* For stroul's error checking */
    char str[BUFSIZ];           /* String to be used as the program argument */
    int i;                      /* Loop index */

    /* Get the name of the program */
    prog_name = *argv;

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
                case OPT_STDIN:
                    opt_stdin = 1;
                    break;
                case OPT_END:
                    argv++;
                    argc--;
                    goto post_options;
                case OPT_NULL:
                    return sieve_error(ERR_EXPECTED_OPT, OPT_START);
                default:
                    return sieve_error(ERR_ILLEGAL_OPTION, c);
            }
        } while ((c = *++*argv));
    }

post_options:

    /* The -u option shouldn't be used without the -f option */
    if (opt_unique && !opt_factor)
        return sieve_error(ERR_U_WITHOUT_F);

    /* Print help message if necessary */
    if (opt_help) {
        printf(HELP_MESSAGE, prog_name,
                OPT_COUNT, OPT_FACTOR, OPT_UNIQUE, OPT_STDIN, OPT_END);
        return EXIT_SUCCESS;
    }

    /* Check whether to get program argument from stdin or the command-line */
    if ((argc > NUM_ARGS) || (argc > 0 && opt_stdin)) {
        /* There are too many arguments -- show error and exit */
        return sieve_error(ERR_TOO_MANY_ARGS);
    } else if (argc < NUM_ARGS) {
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
        for (i = 0; i < BUFSIZ; i++) {
            str[i] = (*argv)[i];
            if (!(*argv)[i])
                break;
        }
    }

    /* Strip potential trailing newline */
    for (i = 0; i < BUFSIZ && str[i]; i++) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }

    /* Look for a minus sign in the argument (this isn't done by strtoul) */
    for (i = 0; i < BUFSIZ && str[i]; i++)
        if (str[i] == MINUS)
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
