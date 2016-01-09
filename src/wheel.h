/*
 * FILE:        wheel.h
 * AUTHOR:      Artem Mavrin
 * UPDATED:     2016-01-09
 * DESCRIPTION: Interface for creating wheels for wheel factorization. The
 *              implementation is in the file wheel.c. This file contains just
 *              the struct and function prototypes necessary for the use of
 *              wheels in other programs.
 */

#ifndef WHEEL_H
#define WHEEL_H

typedef struct Wheel Wheel;

Wheel * newWheel(const unsigned long *, const unsigned long);
void deleteWheel(Wheel **);
unsigned long nextp(Wheel *);

#endif
