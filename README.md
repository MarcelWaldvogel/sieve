# WheelSieve

Wheel-based prime number sieve written in C.
A Python 3 clone of this program is
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
