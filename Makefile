CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O2
DEBUG=-g
ASSEMBLY=-S -fverbose-asm -masm=intel

.PHONY: assembly clean debug default force sieve_speed_test

default: factor sieve

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ -c $<

factor: factor.c wheel.o
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ $^

sieve: sieve.c wheel.o
	$(CC) $(C_FLAGS) $(OPTIMIZE) -o $@ $^

force:
	@make clean
	@make default

debug:
	$(CC) $(DEBUG) -o wheel.o -c wheel.c
	$(CC) $(DEBUG) -o factor factor.c wheel.o
	$(CC) $(DEBUG) -o sieve sieve.c wheel.o

assembly:
	$(CC) $(OPTIMIZE) $(ASSEMBLY) wheel.c
	$(CC) $(OPTIMIZE) $(ASSEMBLY) factor.c
	$(CC) $(OPTIMIZE) $(ASSEMBLY) sieve.c

clean:
	rm -f factor sieve
	rm -f *.o
	rm -rf *.dSYM
	rm -f *.s
