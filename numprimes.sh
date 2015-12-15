#!/usr/bin/env bash

# FILE:         numprimes.sh
# AUTHOR:       Artem Mavrin
# UPDATED:      2015-12-15
# DESCRIPTION:  Uses sieve.o to compute the number of primes less than or equal
#               to a specified positive integer.

if [ $# -eq 1 ]; then
  make --quiet sieve.o
  ./sieve.o $1 | wc -l | sed -e 's/^[ ]*//'
else
  >&2 echo "Compute the number π(N) of primes less than or equal to N."
  >&2 echo "Usage:"
  >&2 echo "$0 N"
  >&2 echo "where N is a positive integer"
fi
