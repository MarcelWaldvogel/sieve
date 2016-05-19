/*
 * FILE:        wheel.c
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-05-19
 * DESCRIPTION: Implementation of wheels for wheel factorization algorithms.
 */

#include <stdlib.h>
#include "wheel.h"

#ifdef DEBUG_ON
#include "debug.h"
#endif

typedef struct Spoke Spoke;

/*
 * STRUCT:      Wheel
 * DESCRIPTION: A circular singly linked list implementation of wheels used in
 *              wheel factorization algorithms. The wheel is generated by a list
 *              of known primes, called the base primes, and the wheel generates
 *              the sequence of the positive integers which are coprime to each
 *              base prime.
 * FIELDS:      circumference (unsigned long): The product of the base primes.
 *              numSpokes (unsigned long): The number of spokes in the wheel,
 *              which is the number of integers between 1 and circumference
 *              which are coprime to circumference.
 *              primeCandidate (long): The biggest positive integer coprime
 *              to each base prime computed so far.
 *              spoke (Spoke *): Pointer to the next spoke in the wheel.
 */
struct Wheel {
    unsigned long circumference;    /* The product of the base primes */
    unsigned long numSpokes;        /* The number of spokes in the wheel */
    Spoke *spoke;                   /* The next spoke in the wheel */
};

/*
 * STRUCT:      Spoke
 * DESCRIPTION: Container for numbers coprime to the base primes used to create
 *              a wheel.
 * FIELDS:      num (unsigned long): The value represented by this spoke. This
 *              number is coprime to each base prime and it is increased after
 *              every full revolution of the wheel.
 *              next (struct Spoke *): The spoke following the current spoke.
 */
struct Spoke {
    unsigned long num;      /* A number coprime to the base primes */
    struct Spoke *next;     /* The next spoke in the wheel */
};

/*
 * FUNCTION:    newWheel
 * DESCRIPTION: Creates a new wheel for identifying prime number candidates. The
 *              wheel is dynamically allocated and must be deallocated with the
 *              deleteWheel function.
 * PARAMETERS:  bp (const unsigned long *): The `base primes': A list of a few
 *              known prime numbers used to create the wheel (e.g., 2, 3, 5, 7).
 *              The numbers are assumed to be distinct primes, but no check is
 *              made to ensure that they are either prime or distinct.
 *              nbp (const unsigned long): The number of base primes.
 * ERRORS:      If memory could not be allocated for the wheel, a null pointer
 *              is returned.
 * RETURNS:     A pointer to the new wheel.
 */
Wheel * newWheel(const unsigned long *bp, const unsigned long nbp) {
    unsigned long num;  /* To be checked for coprimeness */
    unsigned long i;    /* Used to track position in the base primes list */
    Wheel *wheel;       /* The wheel being created */

    /* Allocate memory for the wheel and check if malloc failed */
    wheel = malloc(sizeof(Wheel));
    if (!wheel)
        return NULL;

    /* Initialize the fields of the wheel */
    wheel->circumference = 1;
    wheel->numSpokes = 0;
    wheel->spoke = NULL;

    /* Compute the circumference (product of base primes) of the wheel */
    for (i = 0; i < nbp; i++)
        wheel->circumference *= bp[i];

    /* Create all the spokes in the wheel */
    for (num = 1; num < wheel->circumference; num++) {
        /* Check if the current number is coprime to all the base primes */
        int isCoprime = 1;
        for (i = 0; i < nbp; i++) {
            if (num % bp[i] == 0) {
                isCoprime = 0;
                break;
            }
        }
        if (isCoprime) {
            /* Create a spoke for the current coprime number */
            Spoke *spoke = malloc(sizeof(Spoke));
            /* Delete the wheel and return NULL if malloc failed */
            if (!spoke) {
                deleteWheel(&wheel);
                return NULL;
            }
            /* Assign the current coprime number to this spoke */
            spoke->num = num;
            /* Insert the spoke into the wheel */
            if (!wheel->spoke) {
                /* The current spoke is the first spoke, link it to itself */
                spoke->next = spoke;
            } else {
                /* Link this spoke to the last spoke in the wheel */
                spoke->next = wheel->spoke->next;
                wheel->spoke->next = spoke;
            }
            /* Add the spoke to the end of the wheel */
            wheel->spoke = spoke;
            wheel->numSpokes++;
        }
    }
    /* Go to the first spoke in the wheel */
    wheel->spoke = wheel->spoke->next;

    #ifdef DEBUG_ON
    DEBUG_MSG("New wheel at %p (circum: %lu, spokes: %lu)",
            wheel, wheel->circumference, wheel->numSpokes);
    #endif

    return wheel;
}

/*
 * FUNCTION:    deleteWheel
 * DESCRIPTION: Deallocates all memory associated with a wheel.
 * PARAMETERS:  wpp (Wheel **): A pointer to a pointer to the wheel being
 *              deleted.
 * RETURNS:     Nothing.
 */
void deleteWheel(Wheel **wpp) {
    if (wpp && *wpp) {
        #ifdef DEBUG_ON
        DEBUG_MSG("Deleting wheel at %p ...", *wpp);
        #endif

        /* Deallocate all the spokes in the wheel */
        while ((*wpp)->numSpokes-- > 0) {
            Spoke *spoke = (*wpp)->spoke;
            (*wpp)->spoke = (*wpp)->spoke->next;
            free(spoke);
        }
        /* Deallocate the wheel and make the pointer to it NULL */
        free(*wpp);
        *wpp = NULL;
    }
}

/*
 * FUNCTION:    nextp
 * DESCRIPTION: Get the next prime candidate from the wheel. The first prime
 *              candidate is necessarily the smallest prime number not included
 *              in the list of base primes used to create the wheel.
 * PARAMETERS:  wheel (Wheel *): A pointer to the wheel being used.
 * RETURNS:     The next prime candidate computed by the wheel.
 */
unsigned long nextp(Wheel *wheel) {
    wheel->spoke->num += wheel->circumference;    /* Increment current spoke */
    wheel->spoke = wheel->spoke->next;            /* Move to the next spoke */
    return wheel->spoke->num;
}

/* Compile with -D'TEST' to enable testing of the wheel */
#ifdef TEST

#include <stdio.h>
#include <stdlib.h>

#define COUNT 20    /* Number of prime number candidates to print at a time */

/* Naive trial division primality tester prototype */
static unsigned long iscomp(const unsigned long);

/*
 * FUNCTION:    main
 * DESCRIPTION: Generates MAX prime number candidates using a wheel created with
 *              the command-line arguments as the base primes.
 * PARAMETERS:  argc (int): 1 + the number of command-line arguments
 *              argv (const char **): array of strings, the first being the
 *              program name, and the subsequent ones being the command-line
 *              arguments.
 */
int main(int argc, const char **argv) {
    int max = COUNT;                /* Number of prime candidates to print */
    int i;                          /* Loop index */
    int c;                          /* User command */
    int num_primes = 0;             /* Number of truly prime prime candidates */
    unsigned long p;                /* A prime number candidate */
    unsigned long d;                /* A divisor of a prime number candidate */
    Wheel *wheel;                   /* Wheel being tested */
    unsigned long numBasePrimes;    /* Number of user-specified base primes */
    unsigned long *basePrimes;      /* Array of user-specified base primes */

    if (argc == 1) {
        /* If there are no command-line arguments, print usage message */
        fprintf(stderr,
                "Usage: Enter a list of primes: e.g.,\t\n%s 2 3 5\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Allocate memory for the base primes */
    numBasePrimes = (unsigned long) argc - 1;
    basePrimes = malloc(numBasePrimes * sizeof(unsigned long));

    /* Print the base primes */
    printf("The %lu base primes:\n", numBasePrimes);
    for (i = 1; i < argc; i++) {
        /* Parse command line arguments as primes (no error checking) */
        basePrimes[i - 1] = strtoul(argv[i], NULL, 0);
        printf("Base prime      #%i:\t%4ld\n", i, basePrimes[i - 1]);
    }

    /* Create the wheel to be tested */
    wheel = newWheel(basePrimes, numBasePrimes);

    /* Print the prime candidates */
    printf("The first %i prime candidates:\n", max);
    for (i = 1; i <= max; i++) {
        p = nextp(wheel);
        printf("Prime candidate #%i\t%4ld", i, p);
        if ((d = iscomp(p))) {
            printf(" (divisible by %lu)\n", d);
        } else {
            num_primes++;
            printf("\n");
        }
    }

    while (1) {
        /* Ask if user wants to see more base primes */
        printf("Show %i more prime candidate%s? (Y/n)\t",
                COUNT, (COUNT > 1) ? "s" : "");
        c = getchar();

        if (c == EOF) {
            printf("\n");
            break;
        } else if (c == 'n' || c == 'N') {
            break;
        } else if (c != '\n' && c != 'Y' && c != 'y') {
            /* Flush remaining characters in stdin */
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        max += COUNT;
        for (; i <= max; i++) {
            p = nextp(wheel);
            printf("Prime candidate #%i\t%4ld", i, p);
            if ((d = iscomp(p))) {
                printf(" (divisible by %lu)\n", d);
            } else {
                num_primes++;
                printf("\n");
            }
        }

        /* Flush remaining characters in stdin */
        if (c != '\n')
            while ((c = getchar()) != '\n' && c != EOF);
    }

    /* Print prime statisticts */
    i--;
    fprintf(stderr, "Statistics:\n");
    fprintf(stderr, "\tTotal prime candidates:      \t%d\n", i);
    fprintf(stderr, "\tTruly prime prime candidates:\t%d (%.2f%%)\n",
            num_primes, 100.0 * (float) num_primes / (float) i);

    /* Clean up and return */
    free(basePrimes);
    deleteWheel(&wheel);
    return EXIT_SUCCESS;
}


/*
 * FUNCTION:    Naive trial division primality tester
 * DESCRIPTION: Performs trial division until a prime factor is found or the
 *              specified number is determined to be prime.
 * PARAMETERS:  n (const unsigned long): The number to be checked.
 * RETURNS:     0 if n is prime, otherwise the smallest prime divisor of n.
 */
static unsigned long iscomp(const unsigned long n) {
    unsigned long p;
    if (n % 2 == 0)
        return 2;
    for (p = 3; p * p <= n; p += 2)
        if (n % p == 0)
            return p;
    return 0;
}

#endif /* TEST */
