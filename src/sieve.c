/*
 * FILE:        sieve.c
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Implementation of the sieve of Eratosthenes with wheel
 *              factorization. This creates one of two object files: if the
 *              macro COUNT_PRIMES is defined, it creates an object file
 *              containing the function sieve_count, which returns the number of
 *              primes up to a given number. If the macro COUNT_PRIMES is not
 *              defined, it creates an object file containing the function
 *              sieve_list, which prints the primes up to a given number to
 *              stdout. Exactly one of these two macros must be defined.
 */

#include <stdlib.h>
#include <stdio.h>

#include "bitarray.h"
#include "wheel.h"
#include "sieve.h"

#define ERR_BIT_ALLOCATE    "sieve: bit array"
#define ERR_WHEEL_ALLOCATE  "sieve: wheel"

static const unsigned long base_primes[] = {2, 3, 5, 7, 11, 13};
static const unsigned long num_base_primes = 6;

#ifndef COUNT_PRIMES
/* Function to print an unsigned long to stdout */
static void printul(unsigned long);
#endif

/*
 * FUNCTIONS:   sieve_count/sieve_list
 * DESCRIPTION: Sieve of Eratosthenes algorithm implementation using wheel
 *              factorization. All the prime numbers less than or equal to a
 *              specified nonnegative integer `max' are found and either
 *              (sieve_list) printed to stdout, or (sieve_count) the number of
 *              such primes is returned.
 *              First, the prime 2 is sieved if max >= 2. Then, a bit array is
 *              created, with one bit for every odd integer between 0 and max.
 *              The bit represents whether the odd integer is prime (1) or
 *              composite (0). The list of base primes is sieved first, and
 *              their multiples are marked as composite in the bit array. Then
 *              the remaining odd primes less than or equal to sqrt(max) are
 *              sieved, and their multiples are marked as composite in the bit
 *              array. Finally, the remaining primes between sqrt(max) and max
 *              are sieved.
 * PARAMETERS:  max (const unsigned long): The upper bound for the sieve.
 * RETURNS:     sieve_count: The number of primes less than or equal to max.
 *              sieve_list: Nothing.
 */
#ifdef COUNT_PRIMES
unsigned long sieve_count(const unsigned long max)
#else
void sieve_list(const unsigned long max)
#endif
{
    struct bitarray *is_prime = NULL;   /* Bit array representing primality */
    struct wheel *wheel = NULL;         /* The wheel used in the sieve */
#ifdef COUNT_PRIMES
    unsigned long count = 0;            /* The number of primes */
#endif
    unsigned long prime;                /* A prime candidate */
    unsigned long comp;                 /* A necessarily composite number */
    unsigned long index;                /* Track position in loops */

    /* Create the sieving array. To save space we only store the primality of
     * odd integers, starting with 1 in position 0, 3 in position 1, etc. */
    is_prime = new_bitarray((max + 1) / 2);
    if (!is_prime) {
        perror(ERR_BIT_ALLOCATE);
        goto failure;
    }
    set_all_bits(is_prime); /* Initialize all bits to 1 */
    clear_bit(is_prime, 0); /* 1 is not prime */

    /* Create the wheel */
    wheel = new_wheel(base_primes, num_base_primes);
    if (!wheel) {
        perror(ERR_WHEEL_ALLOCATE);
        goto failure;
    }

    /* Easy case #1: no primes less than 2 */
    if (max < 2)
        goto end;

    /* Now max is guaranteed to be at least 2 */
#ifdef COUNT_PRIMES
    count++;
#else
    printul(2UL);
#endif

    /* Easy case #2: only the even prime */
    if (max == 2)
        goto end;

    /* Now max is guaranteed to be at least 3 */

    /* Sieve the odd base primes */
    for (index = 1; index < num_base_primes; index++) {
        prime = base_primes[index];
        if (prime > max)
            break;
#ifdef COUNT_PRIMES
        count++;
#else
        printul(prime);
#endif
        /* Cross off multiples of the current prime */
        for (comp = prime * prime; comp <= max; comp += 2 * prime)
            clear_bit(is_prime, comp / 2);
    }

    /* Sieve the remaining primes <= sqrt(max) */
    for (prime = nextp(wheel); prime * prime <= max; prime = nextp(wheel)) {
        if (get_bit(is_prime, prime / 2)) {
#ifdef COUNT_PRIMES
            count++;
#else
            printul(prime);
#endif
            /* Cross off multiples of the current prime */
            for (comp = prime * prime; comp <= max; comp += 2 * prime)
                clear_bit(is_prime, comp / 2);
        }
    }

    /* Sieve the remaining primes > sqrt(max) */
    while(prime <= max) {
        if (get_bit(is_prime, prime / 2)) {
#ifdef COUNT_PRIMES
            count++;
#else
            printul(prime);
#endif
        }
        prime = nextp(wheel);
    }

end:
    /* Clean up and return */
    delete_bitarray(&is_prime);
    delete_wheel(&wheel);
#ifdef COUNT_PRIMES
    return count;
#else
    return;
#endif

failure:
    delete_bitarray(&is_prime);
    delete_wheel(&wheel);
    exit(EXIT_FAILURE);
}

#ifndef COUNT_PRIMES
#define BASE 10
#define TOCHAR(d) ('0' + (d))
/*
 * FUNCTION:    printul
 * DESCRIPTION: Prints an unsigned long to stdout (in decimal). This function
 *              automatically appends a newline at the end.
 * PARAMETERS:  n (unsigned long): The number to be printed.
 * RETURNS:     Nothing.
 */
static void printul(unsigned long n) {
    char digits[BUFSIZ];
    char *digit = digits;
    *digit++ = '\0';
    while (n) {
        *digit++ = TOCHAR(n % BASE);
        n /= BASE;
    }
    while (*--digit) {
        putchar(*digit);
    }
    putchar('\n');
}
#endif
