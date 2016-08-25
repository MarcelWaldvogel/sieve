/*
 * FILE:        sieve.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-08-25
 * DESCRIPTION: Sieve of Eratosthenes function prototypes: one function to count
 *              the number of primes up to a given number, and one function to
 *              list the primes up to a given number.
 */

#ifndef SIEVE_H

unsigned long sieve_count(const unsigned long);
void sieve_list(const unsigned long);

#endif
