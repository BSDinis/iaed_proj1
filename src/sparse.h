/*
 *
 * sparse.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the sparse datatype that represents a sparse matrix
 * defines several operations on that datatype
 */

#ifndef SPARSE_H
#define SPARSE_H

#include <stdbool.h>
#include "el.h"
#include "pos.h"

#define MAX_N_ELEM 10000
#define MAX_FILENAME 80 
#define MAX_VAL_DIFF 10000

/* sparse datatype
 *
 * list: list of non-zero elements of the sparse matrix
 * zero: value ommited
 * min: the position which has the smallest column and row
 * max: the position which has the biggest column and row
 * nelem: number of elements <-> first free position in the list
 */
typedef struct {
  el list[MAX_N_ELEM];
  double zero;
  pos min, max;
  unsigned long int nelem;
} sparse;


/* selectors */
#define list(a) ((a).list)
#define zero(a) ((a).zero)
#define min(a) ((a).min)
#define max(a) ((a).max)
#define nelem(a) ((a).nelem)

/* test */
#define empty_sparse(a) (nelem(a) == 0)

/* computes the size of the complete matrix */
#define height_sparse(a) ((unsigned long) (row(max((a))) - row(min((a))) + 1))
#define width_sparse(a) ((unsigned long) (col(max((a))) - col(min((a))) + 1))
#define size_sparse(a) ((unsigned long) width_sparse(a) * height_sparse(a))

/* computes the density of a matrix */
#define density_sparse(a) (((double) nelem(m)) / size_sparse(m))

/* checks if an element's position is inside the limits of a sparse matrix */
#define inside_sparse(m, e) (row(pos(e)) >= row(min(m)) && row(pos(e)) <= row(max(m)) && col(pos(e)) >= col(min(m)) && col(pos(e)) <= col(max(m)))

sparse init_sparse(int n, ...);

void sparse_to_file(sparse m, char filename[]);


/* operations */

bool add_el(sparse *m, el e);

void print_sparse(sparse m);

void print_charact_sparse(sparse m);

void change_zero(sparse *m, double new_zero);

void print_row_i(sparse m, unsigned long row_i);

void print_col_j(sparse m, unsigned long col_j);

void print_all_over_diag(sparse m);

void print_n_diff_vals(sparse m);

#endif /* !SPARSE_H */
