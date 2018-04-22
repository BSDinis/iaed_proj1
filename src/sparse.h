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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>

#include "el.h"
#include "pos.h"
#include "sort.h"


#define MAX_N_ELEM 10000
#define MAX_N_ELEM_BUFF 5 // 10000 has 5 chars
#define MAX_FILENAME 81 // extra character for terminator
#define INIT_SIZE 100
#define MAX_FLOAT_BUFFER 1000

/* sparse datatype
 *
 * list: list of non-zero elements of the sparse matrix
 * zero: value ommited
 * min: the position which has the smallest column and row
 * max: the position which has the biggest column and row
 * nelem: number of elements <-> first free position in the list
 * allocd: current number of allocated elements in the list
 */
typedef struct {
  el list[MAX_N_ELEM];
  double zero;
  pos min, max;
  unsigned int nelem;
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
#define width_sparse(a) ((unsigned) (row(max((a))) - row(min((a))) + 1))
#define height_sparse(a) ((unsigned) (col(max((a))) - col(min((a))) + 1))
#define size_sparse(a) ((unsigned) width_sparse(a) * height_sparse(a))

/* computes the density of a matrix */
#define density_sparse(a) (((double) nelem(m)) / size_sparse(m))

/* constructor */

/* 
 * initializes a matrix
 * may take a filename as input, in which case tries to load 
 * matrix of a *.sm file; if it fails, initializes a new matrix
 * 
 * n: number of extra arguments; in principle should be 0 or 1,
 * but treats any other value as 0, creating a new matrix
 */
sparse init_sparse(int n, ...);

/* destructor */
void free_sparse(sparse m);

/* exporting */
/* converts a matrix into a file */
void sparse_to_file(sparse m, char *filename);

/* operations */

/* adds a new element to the matrix */
int add_el(sparse *m, el e);

/* prints all the elements on a sparse matrix */
void print_sparse(sparse m);

/* prints the carachteristics of a sparse matrix */
void print_charact_sparse(sparse m);

/* changes the value of zero */
void change_zero(sparse *m, double new_zero);

/* sorts a sparse matrix, with regard to either the columns or the rows,
 * depending on a flag */
void sort_sparse(sparse *m, bool col);

#endif /* !SPARSE_H */
