#!/usr/bin/env bash

# This is a test of the sieve program's factoring feature.
# To run the test, type
# ./factor_test.sh <nonnegative integer>

if [[ $# != 1 ]]; then
    echo "Specify an integer to factor"
    exit 1
fi

factor="bin/sieve -f --"
prod=1
for prime in $($factor "$1"); do
    prod=$(($prod * $prime))
done

if [[ $prod == $1 ]]; then
    echo "Test passed"
    echo "$1 factored successfully"
    exit 0
else
    echo "Test failed"
    echo "$1 not factored successfuly. Product of factors: $prod"
    exit 1
fi
