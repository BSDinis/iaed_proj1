/*
 *
 * pos.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the position datatype
 *
 * pos: the position of an element in a sparse matrix; has:
 * a row index, i [unsigned long int],
 * a column index, j [unsigned long int]
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

#include "pos.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define max_int(a, b) ((a > b) ? a : b)
#define min_int(a, b) ((a < b) ? a : b)

/* 
 * constructor 
 * input: two unsigned ints <row> and <col>
 * returns a position
 */
pos init_pos(unsigned long i, unsigned long j)
{
  pos p;
  p.i = i;
  p.j = j;
  return p;
}

/* 
 * input: position and an buffer to be written
 * writes a position in a string
 * format: [<row>;<col>]
 */
void out_pos(pos p, char *out)
{
  sprintf(out, "[%lu;%lu]", row(p), col(p));
}


/* 
 * input: 2 positions
 * return: the max of the two positions (a, b) and (c, d)
 * the max is the position (i, j) so that:
 * i >= a && i >= c
 * j >= b && j >= d
 * where i and j are the minimum values that make the inequalities true
 */
pos max_pos(pos a, pos b)
{
  return init_pos(max_int(row(a), row(b)), max_int(col(a), col(b)));
}

/* 
 * input: 2 positions
 * return: the min of the two positions (a, b) and (c, d)
 * the min is the position (i, j) so that:
 * i <= a && i <= c
 * j <= b && j <= d
 * where i and j are the maximum values that make the inequalities true
 */
pos min_pos(pos a, pos b)
{
  return init_pos(min_int(row(a), row(b)), min_int(col(a), col(b)));
}
