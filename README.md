# WheelSieve

Wheel-based prime number sieve and integer factorization program written in C.
A Python 3 clone of the sieve is
[PySieve](https://github.com/artemmavrin/pysieve).

## Usage

### Setup

To clone this project from GitHub, type the following in a terminal:
```
git clone https://github.com/artemmavrin/wheelsieve.git
cd wheelsieve
make
```
This creates the `bin` directory and the executable `bin/sieve` in the current
directory.
The `bin/sieve` executable takes one or zero arguments and a few different
options. See below for more.

### Listing Primes Up To a Number

To list all the prime numbers less than or equal to a specified nonnegative
integer *N*, type
```
bin/sieve N
```

### Counting the Number of Primes Up To a Number

To count the number of prime numbers less than or equal to a specified
nonnegative integer *N*, use the `-n` option:
```
bin/sieve -n N
```

### Factoring a Number Into Primes

To list the prime factors of a nonnegative integer *N*, use the `-f` option:
```
bin/sieve -f N
```
If a prime number divides *N* multiple times, then it will appear in the list as
many times as it divides *N*. To list each prime only once, use the `-u` option
together with the `-f` option:
```
bin/sieve -f -u N
```

### Counting the Number of Prime Factors of a Number

To count the number of prime factors of a nonnegative number *N*, use the `-n`
and `-f` options together:
```
bin/sieve -f -n N
```
Again, if a given prime number divides *N* multiple times, then it will counted
as many times as it divides *N*. To count each prime factor only once, use the
`-u` option together with the `-f` and `-n` options:
```
bin/sieve -f -n -u N
```

### Reading From Standard Input

The nonnegative integer *N* can be read from `stdin` by using the `-i` option
together with any or none of the other options mentioned above:
```
bin/sieve -i
```
For example,
```
echo 100 | bin/sieve -n -i
```
does the same thing as
```
bin/sieve -n 100
```
Both will count the number of primes less than or equal to 100.

### Show Help

To view a brief description of the WheelSieve program, use the `-h` option:
```
bin/sieve -h
```

### Remark About Options

The options described above can be used in any order and repeated, and they may
be combined into one option.
For example,
```
bin/sieve -f -n -u 123
```
can be written as
```
bin/sieve -nuf 123
```
The `-u` option may not be used without the `-f` option.
Any options (i.e., strings prefixed by `-`) not listed above will cause the
program to print an error message and abort.
