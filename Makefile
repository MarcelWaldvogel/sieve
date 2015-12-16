CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic -Os

.PHONY: clean default numprimes

default: divisors sieve

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) -o $@ -c $<

divisors: divisors.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

sieve: sieve.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	rm -f divisors sieve
	rm -f *.o
	rm -rf *.dSYM
