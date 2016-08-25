# Directories
BIN=bin
OBJ=obj
SRC=src

# Compiler options
CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic
OPTIMIZE=-O3
DEBUG=-g -D'DEBUG_ON'
TEST=-D'TEST'
COUNT=-D'COUNT_PRIMES'

# Object files
OBJ_FILES=${OBJ}/wheel.o ${OBJ}/bitarray.o ${OBJ}/sieve_count.o ${OBJ}/sieve_list.o

.PHONY: clean debug default directories force test

# Create the necessary directories, then the main executable
default: directories ${BIN}/sieve

# Create the main executable from the source and object files
${BIN}/sieve: ${SRC}/main.c ${SRC}/main.h ${OBJ_FILES}
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ ${SRC}/main.c ${OBJ_FILES}

# Create the object files for all the program components
${OBJ}/sieve_count.o: ${SRC}/sieve.c ${SRC}/sieve.h
	${CC} ${CFLAGS} ${OPTIMIZE} ${COUNT} -o $@ -c $<

${OBJ}/sieve_list.o: ${SRC}/sieve.c ${SRC}/sieve.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

${OBJ}/%.o: ${SRC}/%.c ${SRC}/%.h
	${CC} ${CFLAGS} ${OPTIMIZE} -o $@ -c $<

# Compile everything from scratch no matter what
force: clean default

# Create testing executables
test: directories
	${CC} ${CFLAGS} ${DEBUG} ${TEST} -o ${BIN}/wheel_test ${SRC}/wheel.c

# Compile with debug messages turned on
debug: directories
	@rm -rf ${OBJ}/*
	@make OPTIMIZE=-O0 CFLAGS="${CFLAGS} ${DEBUG}"

# Create the binary executable and object file directories
directories:
	@for dir in ${BIN} ${OBJ}; do \
	    if [ ! -d $$dir ]; then \
	        echo "Creating $$dir directory ..."; \
	        mkdir $$dir; \
	    fi; \
	done;

# Remove non-source code
clean:
	rm -rf ${BIN}/*
	rm -rf ${OBJ}/*
	rm -rf core
