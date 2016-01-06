# Directories
BIN=bin
OBJ=obj
SRC=src

# Compiler options
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O3
DEBUG=-g
ASSEMBLY=-S -fverbose-asm -masm=intel

.PHONY: all assembly clean debug directories force

all: directories ${BIN}/factor ${BIN}/sieve

${OBJ}/%.o: ${SRC}/%.c ${SRC}/%.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

${BIN}/factor: ${SRC}/factor.c ${OBJ}/wheel.o
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ $^

${BIN}/sieve: ${SRC}/sieve.c ${OBJ}/wheel.o ${OBJ}/bitarray.o
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ $^

force: clean all

debug:
	${CC} ${DEBUG} -o ${OBJ}/wheel.o -c ${SRC}/wheel.c
	${CC} ${DEBUG} -o ${OBJ}/bitarray.o -c ${SRC}/bitarray.c
	${CC} ${DEBUG} -o ${BIN}/factor ${SRC}/factor.c ${OBJ}/wheel.o
	${CC} ${DEBUG} -o ${BIN}/sieve ${SRC}/sieve.c ${OBJ}/wheel.o ${OBJ}/bitarray.o

assembly:
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/wheel.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/bitarray.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/factor.c
	${CC} ${OPTIMIZE} ${ASSEMBLY} ${SRC}/sieve.c

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
	rm -f *.s
