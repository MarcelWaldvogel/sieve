/*
 * FILE:        main.h
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Defines constants to be used in main.c.
 */

#ifndef MAIN_STRINGS_H
#define MAIN_STRINGS_H

#define PROGRAM_NAME        "sieve"

#define ERR_ILLEGAL_OPTION  "illegal option `-%c'.\n"
#define ERR_EXPECTED_ARG    "expected argument.\n"
#define ERR_TOO_MANY_ARGS   "too many arguments.\n"
#define ERR_CONVERT         "`%s' is not a nonnegative integer.\n"
#define ERR_TOO_LARGE       "%s is too large.\n"
#define ERR_READ_STDIN      "could not read from stdin.\n"
#define ERR_TOO_LONG        "argument too long.\n"
#define ERR_INTERRUPT       "interrupted.\n"
#define ERR_USAGE_HELP      "For help, run `" PROGRAM_NAME " -%c'.\n"

#define HELP_MESSAGE        "\
Wheel-based Sieve of Eratosthenes\n\n\
Usage:\n\
\t" PROGRAM_NAME " [options] <nonnegative integer>\n\n\
Without any options, this will list all the prime numbers less than or equal\n\
to the specified nonnegative integer.\n\n\
Options:\n\
\t-%c\tShow only the number of primes.\n\
\t-%c\tRead the nonnegative integer from stdin instead of from the\n\
\t\tcommand-line.\n"

#define OP_HELP     'h'     /* Option to print help message */
#define OP_COUNT    'n'     /* Option to print the number of primes */
#define OP_STDIN    'i'     /* Option to read argument from stdin */
#define ALL_OPS     "hni"   /* All options of the program */

#define NUM_ARGS    1       /* Expected number of command-line arguments */
#define BASE        0       /* For stroul - accept decimal, octal, and hex */
#define COUNT_FMT   "%lu\n" /* Format of count output */
#define MINUS       '-'     /* A minus sign -- used for validating input */

#endif
