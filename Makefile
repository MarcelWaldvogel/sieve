CC=gcc
C_FLAGS=-Wall -Wextra -Werror -pedantic -Os

.PHONY: clean default

default: wheel_test.o divisors.o

wheel.o: wheel.c wheel.h
	$(CC) $(C_FLAGS) -o $@ -c $<

wheel_test.o: wheel_test.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

divisors.o: divisors.c wheel.o
	$(CC) $(C_FLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -rf *.dSYM
