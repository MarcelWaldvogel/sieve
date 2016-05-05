# Directories
BIN=bin
OBJ=obj
SRC=src
TEST=test

# Compiler options
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O3
DEBUG=-g -D'DEBUG_ON'
TEST=-D'TEST'
ASSEMBLY=-S -fverbose-asm -masm=intel

# Object files
OBJ_FILES=${OBJ}/wheel.o ${OBJ}/bitarray.o ${OBJ}/sieve.o ${OBJ}/factor.o

.PHONY: all assembly clean debug directories force test

all: directories ${BIN}/sieve

${BIN}/sieve: ${SRC}/main.c ${SRC}/main.h ${OBJ_FILES}
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ ${SRC}/main.c ${OBJ_FILES}

${OBJ}/%.o: ${SRC}/%.c
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

force: clean all

test: ${BIN}/wheel_test

${BIN}/wheel_test: ${SRC}/wheel.c
	${CC} ${DEBUG} ${TEST} -o $@ $<

debug: directories
	${CC} ${DEBUG} -o ${OBJ}/wheel.o -c ${SRC}/wheel.c
	${CC} ${DEBUG} -o ${OBJ}/bitarray.o -c ${SRC}/bitarray.c
	${CC} ${DEBUG} -o ${OBJ}/sieve.o -c ${SRC}/sieve.c
	${CC} ${DEBUG} -o ${OBJ}/factor.o -c ${SRC}/factor.c
	${CC} ${DEBUG} -o ${BIN}/sieve ${SRC}/main.c ${OBJ_FILES}

assembly:
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/wheel.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/bitarray.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/factor.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/sieve.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/main.c

directories:
	@for dir in ${BIN} ${OBJ}; do \
	    if [ ! -d $$dir ]; then \
	        echo "Creating $$dir directory ..."; \
	        mkdir $$dir; \
	    fi; \
	done;

clean:
	rm -rf ${BIN}/*
	rm -rf ${OBJ}/*
	rm -f *.i
	rm -f *.s
	rm -f *.bc
	rm -f *.o
	rm -rf core
	find . -name "*.swp" -type f -delete
