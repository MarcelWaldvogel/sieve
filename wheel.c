/*******************************************************************************
FILE:           wheel.c
AUTHOR:         Artem Mavrin
UPDATED:        2015-12-25
DESCRIPTION:    Implementation of wheels for wheel factorization algorithms.
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "wheel.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct Spoke Spoke;


/*******************************************************************************
STRUCT NAME:    Wheel
DESCRIPTION:    A circular singly linked list implementation of wheels used in
                wheel factorization algorithms. The wheel is generated by a list
                of known primes, called the base primes, and the wheel generates
                the sequence of the positive integers which are coprime to each
                base prime.
FIELDS:         circumference (unsigned long): The product of the base primes.
                numSpokes (unsigned long): The number of spokes in the wheel,
                which is the number of integers between 1 and circumference
                which are coprime to circumference.
                primeCandidate (long): The biggest positive integer coprime
                to each base prime computed so far.
                spoke (Spoke *): Pointer to the next spoke in the wheel.
*******************************************************************************/
struct Wheel {
    unsigned long circumference;    /* The product of the base primes */
    unsigned long numSpokes;        /* The number of spokes in the wheel */
    unsigned long primeCandidate;   /* The last prime candidate computed */
    Spoke *spoke;                   /* The next spoke in the wheel */
};


/*******************************************************************************
STRUCT NAME:    Spoke
DESCRIPTION:    Container for numbers coprime to the base primes used to create
                a wheel.
FIELDS:         num (unsigned long): The value represented by this spoke. This
                number is coprime to each base prime and it is increased after
                every full revolution of the wheel.
                next (struct Spoke *): The spoke following the current spoke.
*******************************************************************************/
struct Spoke {
    unsigned long num;      /* A number coprime to the base primes */
    struct Spoke *next;     /* The next spoke in the wheel */
};


/* "Private" function declarations -- not declared in wheel.h */
static unsigned char isCoprime(const unsigned long, const unsigned long *,
    const unsigned long);


/*******************************************************************************
FUNCTION NAME:  newWheel
DESCRIPTION:    Creates a new wheel for identifying prime number candidates. The
                wheel is dynamically allocated and must be deallocated with the
                deleteWheel function.
PARAMETERS:     basePrimes (const unsigned long *): A list of a few known prime
                numbers used to create the wheel (e.g., 2, 3, 5, 7). The numbers
                are assumed to be distinct primes, but no check is made to
                ensure that they either prime or distinct.
                numBasePrimes (const unsigned long): The number of base primes.
RETURNS:        A pointer to the new wheel.
*******************************************************************************/
Wheel * newWheel(const unsigned long *basePrimes,
    const unsigned long numBasePrimes) {
    /* Step 0 -- Local variable declarations */
    unsigned long circumference;    /* The product of the base primes */
    unsigned long numSpokes;        /* The number of spokes in the wheel */
    unsigned long *coprimes;        /* Integers coprime to the base primes */
    unsigned long num;              /* To be checked for coprimeness */
    unsigned long index;            /* Used to track position inside lists */
    Wheel *wheel;                   /* The wheel being created */

    /* Step 1 -- Compute the circumference and the number of spokes.
     * The circumference of the wheel is the product of the base primes used to
     * create the wheel. The number of spokes is the number of coprime integers
     * 1 <= k <= circumference which are coprime to circumference. This number
     * is the Euler totient function phi(circumference). Since the
     * circumference is a product of distinct primes p_1, p_2, ..., p_n, we have
     * phi(circumference) = (p_1-1)(p_2-1)...(p_n-1).
     */
    circumference = 1;
    numSpokes = 1;
    for (index = 0; index < numBasePrimes; index++) {
        circumference *= basePrimes[index];
        numSpokes *= basePrimes[index] - 1;
    }

    /* Step 2 -- Create the list of integers coprime to the base primes */
    coprimes = (unsigned long *) malloc(numSpokes * sizeof(unsigned long));
    index = 0;
    for (num = 1; num <= circumference; num++)
        if (isCoprime(num, basePrimes, numBasePrimes))
            coprimes[index++] = num;

    /* Step 3 -- Construct the wheel */
    /* Allocate memory for the wheel and initialize all the fields */
    wheel = (Wheel *) malloc(sizeof(Wheel));
    wheel->circumference = circumference;
    wheel->numSpokes = numSpokes;
    wheel->primeCandidate = 1;
    wheel->spoke = NULL;
    /* Create all the spokes in the wheel */
    for (index = 0; index < numSpokes; index++) {
        /* Allocate memory for the spoke and initialize all the fields */
        Spoke *spoke = (Spoke *) malloc(sizeof(Spoke));
        spoke->num = coprimes[index];
        if (!wheel->spoke) {
            /* The current spoke is the first spoke, so link it to itself */
            spoke->next = spoke;
        } else {
            /* Link this spoke to the last spoke in the wheel */
            spoke->next = wheel->spoke->next;
            wheel->spoke->next = spoke;
        }
        /* Add the spoke to the end of the wheel */
        wheel->spoke = spoke;
    }
    /* Go to the first spoke in the wheel */
    wheel->spoke = wheel->spoke->next;

    /* Step 4 -- Clean up and return */
    free(coprimes);
    return wheel;
}


/*******************************************************************************
FUNCTION NAME:  deleteWheel
DESCRIPTION:    Deallocates all memory associated with a wheel.
PARAMETERS:     wpp (Wheel **): A pointer to a pointer to the wheel being
                deleted.
RETURNS:        Nothing.
*******************************************************************************/
void deleteWheel(Wheel **wpp) {
    if (wpp && *wpp) {
        /* Deallocate all the spokes in the wheel */
        while ((*wpp)->numSpokes-- > 0) {
            Spoke *current = (*wpp)->spoke;
            (*wpp)->spoke = (*wpp)->spoke->next;
            free(current);
        }
        /* Deallocate the wheel and make the pointer to it NULL */
        free(*wpp);
        *wpp = NULL;
    }
}


/*******************************************************************************
FUNCTION NAME:  nextp
DESCRIPTION:    Get the next prime candidate from the wheel. The first prime
                candidate is necessarily the smallest prime number not included
                in the base primes used to create the wheel.
PARAMETERS:     wheel (Wheel *): A pointer to the wheel being used.
RETURNS:        The next prime candidate computed by the wheel.
*******************************************************************************/
unsigned long nextp(Wheel *wheel) {
    wheel->primeCandidate -= wheel->spoke->num;
    wheel->spoke->num += wheel->circumference;    /* Increment current spoke */
    wheel->spoke = wheel->spoke->next;            /* Move to the next spoke */
    wheel->primeCandidate += wheel->spoke->num;
    return wheel->primeCandidate;
}


/*******************************************************************************
FUNCTION NAME:  isCoprime ("private": not declared in wheel.h)
DESCRIPTION:    Check whether a given integer is coprime to each base prime.
PARAMETERS:     num (const unsigned long): The number whose coprimeness will be
                checked.
                basePrimes (const long *): The base primes. No error checking is
                performed to ensure that this pointer is not NULL.
                numBasePrimes (const long): The number of base primes.
RETURNS:        TRUE if the number if coprime to each base prime,
                FALSE otherwise.
*******************************************************************************/
static unsigned char isCoprime(const unsigned long num,
    const unsigned long *basePrimes, const unsigned long numBasePrimes) {
    /* Loop through each base prime, checking if it divides num */
    unsigned long i;
    for (i = 0; i < numBasePrimes; i++)
        if (num % *basePrimes++ == 0)
            return FALSE;
    return TRUE;
}
