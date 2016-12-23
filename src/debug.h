/*
 * FILE:        debug.h
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Provides debugging macros. This project's Makefile contains a
 *              `debug' target which, among other things, compiles the source
 *              code with an additional preprocessor macro: DEBUG_ON. All
 *              debugging is intended to happen in the following form:
 *              #ifdef DEBUG_ON
 *                  <debugging code>
 *              #endif
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG_ON
#include <stdio.h>

/*
 * MACRO:       DEBUG_MSG(...)
 * DESCRIPTION: Print a message to stderr along with the file name and line
 *              number. The fprintf statements are surrounded by do { ... }
 *              while (0) to enforce using a semicolon and avoid potential scope
 *              problems caused by code like
 *
 *                  if (...)
 *                      DEBUG_MSG(...);
 *
 *              This macro should be used like printf, except a newline is
 *              placed at the end automatically.
 * ARGUMENTS:   Variadic arguments like printf (i.e., format string + zero or
 *              more values to format, all comma-separated)
 */
#define DEBUG_MSG(...) \
    do { \
        fprintf(stderr, "[DEBUG] %s:%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__); \
        putc('\n', stderr); \
    } while (0)

#else /* DEBUG_ON undefined */
#define DEBUG_MSG(...)
#endif

#endif
