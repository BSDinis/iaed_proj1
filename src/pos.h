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
 * has a line index, i [unsigned int], and a column index, j [unsigned int]
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

typedef struct {
  unsigned int i, j;
} pos;

/* selectors */
#define line(a) (a.i)
#define col(a) (a.j)

/* equality */
#define eq_pos(a, b) (a.i == b.i && a.j == b.j)

/* constructor */
pos init_pos(unsigned int i, unsigned int j);
