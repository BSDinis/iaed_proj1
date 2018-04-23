/*
 *
 * pos.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * position datatype header file
 *
 * pos: the position of an element in a sparse matrix
 * has a row index, i [unsigned int], and a column index, j [unsigned int]
 *
 * note on the data abstraction: this datatype is simple a tuple with 2 
 * entries that represents a position in the sparse matrix
 *
 * the entire program is written under the assumption that only matrices 
 * will be handled.
 *
 * this means that the abstraction to a n-dimensional tensor will imply 
 * several changes in the program
 */

#ifndef POS_H
#define POS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* buffer for output string for position
 * UINT_MAX = 4294967295 :: 10 digits
 * 2 uints + delimiters + terminator ~ 25 chars
 */
#define BUFFER_OUT_POS 25

typedef struct {
  unsigned int i, j;
} pos;

/* selectors */
#define row(a) (a.i)
#define col(a) (a.j)

/* equality */
#define eq_pos(a, b) (row(a) == row(b) && col(a) == col(b))

/* constructor */
pos init_pos(unsigned int i, unsigned int j);

/* output string:
 * exits with error if the buffer is exceeded  
 * (out must be initialized with the same size)
 */
void out_pos(pos p, char *out);

/* maximum and minimum */
pos max_pos(pos a, pos b);
pos min_pos(pos a, pos b);

#endif /* POS_H */
