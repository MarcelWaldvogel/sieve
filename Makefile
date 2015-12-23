CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic -Os

.PHONY: clean default sieve_speed_test

default: wheel_test.o factor sieve

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) -o $@ -c $<

wheel_test.o: wheel_test.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

factor: factor.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

sieve: sieve.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

sieve_speed_test: sieve
	@echo "1 million"
	time ./sieve -n 1000000
	@echo "10 million"
	time ./sieve -n 10000000
	@echo "100 million"
	time ./sieve -n 100000000
	@echo "1 billion"
	time ./sieve -n 1000000000
	@echo "10 billion"
	time ./sieve -n 10000000000
	@echo "100 billion"
	time ./sieve -n 100000000000

clean:
	rm -f divisors sieve
	rm -f *.o
	rm -rf *.dSYM
