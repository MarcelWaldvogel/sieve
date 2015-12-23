CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O2
DEBUG=-g

.PHONY: clean debug default force sieve_speed_test

default: wheel_test.o factor sieve

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ -c $<

wheel_test.o: wheel_test.c wheel.o
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ $^

factor: factor.c wheel.o
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ $^

sieve: sieve.c wheel.o
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ $^

force:
	@make clean
	@make default

debug:
	@make clean
	$(CC) $(DEBUG) -o wheel.o -c wheel.c
	$(CC) $(DEBUG) -o wheel_test.o wheel_test.c wheel.o
	$(CC) $(DEBUG) -o factor factor.c wheel.o
	$(CC) $(DEBUG) -o sieve sieve.c wheel.o

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
	rm -f factor sieve
	rm -f *.o
	rm -rf *.dSYM
