/*
 * FILE:        debug.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-04-23
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

#include <stdio.h>

/* Print a message to stderr along with the file name and line number */
#define DEBUG_MSG(m, ...) \
    fprintf(stderr, "[DEBUG] %s:%d: " m "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
