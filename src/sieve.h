/*
 * FILE:        sieve.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-08-05
 * DESCRIPTION: Sieve of Eratosthenes function prototypes: one function to count
 *              the number of primes up to a given number, and one function to
 *              list the primes up to a given number.
 */

#ifndef SIEVE_H
#define SIEVE_H

#ifdef COUNT_PRIMES
unsigned long sieve_count(const unsigned long);
#endif

#ifdef LIST_PRIMES
void sieve_list(const unsigned long);
#endif

#endif
