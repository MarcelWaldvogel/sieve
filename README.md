# c-primes
Prime number sieve and integer factorization programs written in C.

## Usage

In the terminal:

    git clone https://github.com/artemmavrin/c-primes.git
    cd c-primes
    make

This creates two executables: `divisors` and `sieve`.

### `divisors`

List the prime divisors of a positive integer.
![divisors screenshot](images/divisors_example.png)

### `sieve`

List the prime numbers less than or equal to a positive integer.
![sieve screenshot](images/sieve_example.png)
Optionally, use the `-n` flag to show only the number of primes.
![sieve performance](images/sieve_performance.png)
