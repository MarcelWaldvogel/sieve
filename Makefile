# Directories
BIN=bin
OBJ=obj
SRC=src
TEST=test

# Compiler options
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O3
DEBUG=-g
ASSEMBLY=-S -fverbose-asm -masm=intel

.PHONY: all assembly clean debug directories force test

all: directories ${BIN}/sieve

${BIN}/sieve: ${SRC}/main.c ${OBJ}/bitarray.o ${OBJ}/wheel.o ${OBJ}/sieve.o ${OBJ}/factor.o
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ $^

${OBJ}/bitarray.o: ${SRC}/bitarray.c ${SRC}/bitarray.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

${OBJ}/wheel.o: ${SRC}/wheel.c ${SRC}/wheel.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

${OBJ}/sieve.o: ${SRC}/sieve.c ${SRC}/sieve.h ${SRC}/bitarray.h ${SRC}/wheel.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

${OBJ}/factor.o: ${SRC}/factor.c ${SRC}/factor.h ${SRC}/wheel.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

force: clean all

test: directories ${TEST}/wheel_test

${TEST}/wheel_test: ${SRC}/${TEST}/wheel_test.c ${SRC}/wheel.c
	${CC} ${CFLAGS} ${DEBUG} -o ${OBJ}/wheel.o -c ${SRC}/wheel.c
	${CC} ${CFLAGS} ${DEBUG} -o $@ $< ${OBJ}/wheel.o

debug:
	${CC} ${DEBUG} -o ${OBJ}/bitarray.o -c ${SRC}/bitarray.c
	${CC} ${DEBUG} -o ${OBJ}/wheel.o -c ${SRC}/wheel.c
	${CC} ${DEBUG} -o ${OBJ}/sieve.o -c ${SRC}/sieve.c
	${CC} ${DEBUG} -o ${OBJ}/factor.o -c ${SRC}/factor.c
	${CC} ${DEBUG} -o ${BIN}/sieve ${SRC}/main.c ${OBJ}/bitarray.o ${OBJ}/wheel.o ${OBJ}/sieve.o ${OBJ}/factor.o
	${CC} ${DEBUG} -o ${TEST}/wheel_test ${SRC}/${TEST}/wheel_test.c ${OBJ}/wheel.o

assembly:
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/bitarray.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/wheel.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/factor.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/sieve.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/main.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/${TEST}/wheel_test.c

directories:
	@for dir in ${BIN} ${OBJ} ${TEST}; do \
		if [ ! -d $$dir ]; then \
			echo "Creating $$dir directory ..."; \
			mkdir $$dir; \
		fi; \
	done;

clean:
	rm -rf ${BIN} ${OBJ} ${TEST}
	rm -f ${SRC}/*.swp
	rm -f *.swp
	rm -f *.s
