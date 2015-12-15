CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic -Os

.PHONY: clean default numprimes

default: divisors.o sieve.o numprimes.sh

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) -o $@ -c $<

divisors.o: divisors.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

sieve.o: sieve.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

numprimes.sh: sieve.o
	chmod +x $@

clean:
	rm -f *.o
	rm -rf *.dSYM
