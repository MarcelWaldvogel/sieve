CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic -Os

.PHONY: clean default numprimes

default: divisors.o sieve.o

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) -o $@ -c $<

divisors.o: divisors.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

sieve.o: sieve.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -rf *.dSYM
