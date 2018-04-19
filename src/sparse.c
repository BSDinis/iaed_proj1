/*
 *
 * sparse.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the sparse datatype that represents a sparse matrix
 * defines several operations on that datatype
 *
 * dependencies:
 *   pos
 *   el
 *   read_write
 */

#include "sparse.h"

/* creates new instance of a sparse matrix */
static sparse init_new_sparse();

/* loads a sparse matrix */
static sparse load_sparse(char *filename);

/* computes the density of a matrix */
double density(sparse m)
{
  unsigned int size = (col(max(m)) - col(min(m)) + 1) * (row(max(m)) - row(min(m)) + 1);
  return ((double) nelem(m)) / size;
}

/* 
 * initializes a matrix
 * may take a filename as input, in which case tries to load 
 * matrix of a *.sm file; if it fails, initializes a new matrix
 */
sparse init_sparse(int n, ...)
{
  va_list valist;
  va_start(valist, n);

  /* if there is one argument, a valid file, load the matrix */
  if (n == 1) {
    char *filename = va_arg(valist, char *);
    va_end(valist);
    if (valid_sm_file(filename)) {
      return load_sparse(filename);
    }
  }

  /* in any other case, create a new instance of the matrix */
  va_end(valist);
  return init_new_sparse();
}

/* 
 * creates new instance of a sparse matrix 
 *
 * the list starts with 100 elements, growing by a factor of 10
 * each time it is necessary, until it reaches the maximum size
 *
 * by omission, the zero is 0 and the min and max position are 
 * the same: (0, 0).
 */
static sparse init_new_sparse()
{
  sparse m;

  allocd(m) = 100;
  list(m) = malloc(allocd(m) * sizeof(el));
  zero(m) = 0;
  nelem(m) = 0; 
  min(m) = init_pos(0, 0);
  max(m) = init_pos(0, 0);

  return m;
}

/* loads a sparse matrix */
static sparse load_sparse(char *filename)
{
  char **input;
  sparse m;
  int i;

  /* check for successful input operation */
  if (file_to_sparse(filename, &input) == 0) {
    sscanf(input[0], "%d", &allocd(m));
    sscanf(input[1], "%d", &nelem(m));
    sscanf(input[2], "%lf", &zero(m));

    list(m) = malloc(allocd(m) * sizeof(el));

    /* if the file has no elements, get the zero and initialize 
     * an empty matrix and exit*/
    if (nelem(m) == 0) {
      min(m) = init_pos(0, 0);
      max(m) = init_pos(0, 0);

    }
    else {
      /* the first element initializes the max and min positions
       * of the matrix, which are updated by every new input */
      list(m)[0] = str_to_el(input[3]);
      min(m) = pos(list(m)[0]);
      max(m) = pos(list(m)[0]);
    }

    for (i = 1; i < nelem(m); i++) {
      list(m)[i] = str_to_el(input[3 + i]);
      max(m) = max_pos(max(m), pos(list(m)[i]));
      min(m) = min_pos(min(m), pos(list(m)[i]));
    }

    /* file_to_sparse allocs nelem + 3 (char*) */
    for (i = 0; i < 3 + nelem(m); free(input[i++]));
    free(input);
    return m;
  }

  /* else returns empty matrix */
  return init_new_sparse();
}




/*
 * given a matrix, returns a list of strings
 * the first three correspond to the allocated memory, number 
 * of elements, and value of zero, respectively
 *
 * the rest correspond to the elements, in the format of the
 * element datatype
 *
 */
char **out_sparse(sparse m)
{
  char **out;
  int i;
  
  out = malloc((nelem(m) + 3) * sizeof(char *));
  snprintf(out[0], MAX_N_ELEM_BUFF + 1, "%u", allocd(m));
  snprintf(out[1], MAX_N_ELEM_BUFF + 1, "%u", nelem(m));
  snprintf(out[2], MAX_FLOAT_BUFFER, "%lf", zero(m));

  for (i = 0; i < nelem(m); i++) {
    out[i + 3] = out_el(list(m)[i]);
  }
  
  return out;
}


/*
 * TODO
 * adds a new element to the matrix
 * if the value is 'zero', removes the value in that position
 * if the position has a value, replace it.
 * may have to reallocate memory for the list
 * 
 * Return codes:
 *   0: Successful addition
 *   1: Successful removal
 *   2: Successful substitution
 *   3: Space limit reached
 */


