/*
 * FILE:        main.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-15
 * DESCRIPTION: Defines constants to be used in main.c.
 */

#ifndef MAIN_STRINGS_H
#define MAIN_STRINGS_H

#define ERR_ILLEGAL_OPTION  "sieve: illegal option `-%c'.\n"
#define ERR_EXPECTED_OPT    "sieve: expected option after `%c'.\n"
#define ERR_EXPECTED_ARG    "sieve: expected argument.\n"
#define ERR_TOO_MANY_ARGS   "sieve: too many arguments.\n"
#define ERR_U_WITHOUT_F     "sieve: the -u option cannot be used without -f.\n"
#define ERR_CONVERT         "sieve: `%s' is not a nonnegative integer.\n"
#define ERR_TOO_LARGE       "sieve: %s is too large.\n"
#define ERR_READ_STDIN      "sieve: could not read from stdin.\n"
#define ERR_TOO_LONG        "sieve: argument too long.\n"
#define ERR_USAGE_HELP      "For help, run %s -%c.\n"

#define HELP_MESSAGE        "WheelSieve\t\
Wheel-based Sieve of Eratosthenes and prime factorization\n\n\
Usage:\n\
\t%s [options] <nonnegative integer>\n\n\
Without any options, this will list all the prime numbers less than or equal\n\
to the specified nonnegative integer.\n\n\
Options:\n\
\t-%c\tShow only the number of primes.\n\
\t-%c\tFactor the given integer. Off by default.\n\
\t-%c\tWhen using the factor option, ignore multiplicity.\n\
\t-%c\tRead the nonnegative integer from stdin instead of from the\n\
\t\tcommand-line.\n"

#define OPT_HELP    'h'     /* Option to print help message */
#define OPT_FACTOR  'f'     /* Option to factor instead of sieve */
#define OPT_COUNT   'n'     /* Option to print the number of primes */
#define OPT_UNIQUE  'u'     /* Option to ignore multiplicity */
#define OPT_STDIN   'i'     /* Option to read argument from stdin */
#define ALL_OPTS    "hfnui" /* All options of the program */

#define NUM_ARGS    1       /* Expected number of command-line arguments */
#define BASE        0       /* For stroul - accept decimal, octal, and hex */
#define COUNT_FMT   "%lu\n" /* Format of count output */
#define MINUS       '-'     /* A minus sign -- used for validating input */

#endif
