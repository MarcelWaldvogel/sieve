/*
 * FILE:        main_strings.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-01-16
 * DESCRIPTION: Defines string constants to be used in main.c.
 */

#ifndef MAIN_STRINGS_H
#define MAIN_STRINGS_H

#define ERR_ILLEGAL_OPTION  "sieve: illegal option `%c'\n"
#define ERR_EXPECTED_ARG    "sieve: expected argument.\n"
#define ERR_TOO_MANY_ARGS   "sieve: too many arguments.\n"
#define ERR_U_WITHOUT_F     "sieve: the -u option cannot be used without -f\n"
#define ERR_USAGE_HELP      "For help, run %s -h\n"

#define HELP_MESSAGE        "WheelSieve\t\
Wheel-based Sieve of Eratosthenes and prime factorization\n\n\
Usage:\n\t%s [-f] [-n] [-u] <positive integer>\n\n\
Without any options, this will list all the prime numbers less than or equal\n\
to the specified positive integer.\n\n\
Options:\n\
\t-n\tShow only the number of primes.\n\
\t-f\tFactor the given integer. Off by default.\n\
\t-u\tWhen using the -f option, ignore multiplicity\n"

#endif
