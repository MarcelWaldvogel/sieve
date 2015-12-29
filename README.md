# WheelSieve

Prime number sieve and integer factorization programs written in C.
A Python3 clone of the sieve is
[PySieve](https://github.com/artemmavrin/pysieve).

## Usage

In the terminal:

    git clone https://github.com/artemmavrin/wheelsieve.git
    cd wheelsieve
    make

This creates the `bin` directory and two executables: `bin/factor` and
`bin/sieve`.

### `factor`

List the prime factors of a positive integer.
Optionally, use the `-n` flag to show only the number of prime factors, and the
`-u` flag to show only the unique prime factors (i.e., ignoring multiplicty).
The `-n` and `-u` flags can be combined to show only the number of distinct
prime factors.
![factor screenshot](images/factor_example.png)

### `sieve`

List the prime numbers less than or equal to a positive integer.
Optionally, use the `-n` flag to show only the number of primes.
![sieve screenshot](images/sieve_example.png)
