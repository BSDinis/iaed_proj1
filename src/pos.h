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

#pragma once

// TODO: search for a more precise value, in conformity with specs
/* buffer for output string */
#define BUFFER_OUT_POS 10000

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

/* output string: exits with error if the output string exceeds buffer */
char *out_pos(pos p);

/* valid string representation of an element */
bool valid_pos(char *str);

/* input from string */
pos str_to_pos(char *input);

/* maximum and minimum */
pos max_pos(pos a, pos b);
pos min_pos(pos a, pos b);
