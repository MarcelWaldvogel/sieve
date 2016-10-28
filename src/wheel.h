/*
 * FILE:        wheel.h
 * AUTHOR:      Artem Mavrin
 * DESCRIPTION: Interface for creating wheels for wheel factorization. The
 *              implementation is in the file wheel.c. This file contains just
 *              the struct and function prototypes necessary for the use of
 *              wheels in other programs.
 */

#ifndef WHEEL_H
#define WHEEL_H

struct wheel * new_wheel(const unsigned long *, const unsigned long);
void delete_wheel(struct wheel **);
unsigned long nextp(struct wheel *);

#endif
