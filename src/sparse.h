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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#include "el.h"
#include "pos.h"
#include "read_write.h"
#include "compress.h"
#include "sort.h"


#define MAX_N_ELEM 10000
#define MAX_N_ELEM_BUFF 5 // 10000 has 5 chars
#define MAX_NAME 80
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
  el *list;
  double zero;
  pos min, max;
  unsigned int nelem;
  unsigned int allocd;
} sparse;


/* selectors */
#define list(a) (a.list)
#define zero(a) (a.zero)
#define min(a) (a.min)
#define max(a) (a.max)
#define nelem(a) (a.nelem)
#define allocd(a) (a.allocd)

/* test */
#define empty_matrix(a) (nelem(a) == 0)

/* computes the density of a matrix */
double density(sparse m);

/* 
 * initializes a matrix
 * may take a filename as input, in which case tries to load 
 * matrix of a *.sm file; if it fails, initializes a new matrix
 * 
 * n: number of extra arguments; in principle should be 0 or 1,
 * but treats any other value as 0, creating a new matrix
 */
sparse init_sparse(int n, ...);


/*
 * given a matrix, returns a list of strings
 * the first two correspond to the allocated memory and number 
 * of elements, respectively
 *
 * the rest correspond to the elements, in the format of the
 * element datatype
 */
char **out_sparse(sparse m);
