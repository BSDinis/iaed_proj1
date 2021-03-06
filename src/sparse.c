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
 *   sort
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#include "sort.h"
#include "sparse.h"

#define max_int(a, b) ((a > b) ? a : b)
#define min_int(a, b) ((a < b) ? a : b)

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

static sparse init_new_sparse();

static sparse file_to_sparse(char *filename);

static void remove_at(sparse *m, unsigned long index);

static void update_max_min(sparse *m);

static void remove_mult(sparse *m, int indices[], int len);


/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/


/* 
 * inputs: variable;
 *   case n != 1: initializes empty sparse (isn't n == 0, because of paranoia)
 *   case n == 1: initializes a sparse from a file, and the filename is the input
 *
 * return: sparse matrix
 */
sparse init_sparse(int n, ...)
{
  va_list valist;
  va_start(valist, n);

  /* if there is one argument, a filename, load the matrix */
  if (n == 1) {
    char *filename = va_arg(valist, char *);
    va_end(valist);
    return file_to_sparse(filename);
  }

  /* in any other case, create a new instance of the matrix */
  va_end(valist);
  return init_new_sparse();
}

/* 
 * returns:creates new instance of a sparse matrix 
 *
 * by omission:
 *   the zero is 0
 *   the max is (0, 0)
 *   the min is (ULONG_MAX, ULONG_MAX)
 */
static sparse init_new_sparse()
{
  sparse m;

  zero(m) = 0;
  nelem(m) = 0; 
  min(m) = init_pos(ULONG_MAX, ULONG_MAX);
  max(m) = init_pos(0, 0);

  return m;
}


/* 
 * input: filename string
 * return: sparse
 *
 * converts a file to a sparse matrix
 */
static sparse file_to_sparse(char *filename)
{
  FILE *fp;
  sparse m = init_new_sparse();
  char input[BUFFER_OUT_EL];

  fp = fopen(filename, "r");

  /* check if file exists */
  if (fp == NULL) {
    fclose(fp);
    return m;
  }

  /* get the input until
   * the maxsize of the sparse is reached 
   * the fgets unsuccessfully returns */
  while (fgets(input, BUFFER_OUT_EL, fp) != NULL 
      && add_el(&m, str_to_el(input)));

  fclose(fp);
  return m;
}


/*
 * inputs: sparse matrix, name of the file in which to write
 * converts a matrix into a file
 */
void sparse_to_file(sparse m, char *filename)
{                                                                   
  int i;                                                            
  FILE *fp;
  char str[BUFFER_OUT_EL];

  fp = fopen(filename, "w");
  
  for (i = 0; i < nelem(m); i++) {
    save_el(list(m)[i], str);
    fprintf(fp, "%s\n", str);
  }

  fclose(fp);
}


/*
 * input: ptr to sparse, element to add
 *
 * adds a new element to the matrix
 *
 * if the value is 'zero', removes the value in that position
 * if the position has a value, replace it.
 * 
 * Return codes:
 *   true: Successful operation
 *   false: Space limit reached
 */
bool add_el(sparse *m, el e)
{
  int i;

  /* check if there is an element with the same position in the matrix
   * if there is, replace or remove and exit.
   * important property: there aren't repeated positions */
  if (inside_sparse(*m, e)) {
    for (i = 0; i < nelem(*m) && !eq_pos(pos(e), pos(list(*m)[i])); i++);

    if (i != nelem(*m)) {
      if(val(e) == zero(*m)) {
        remove_at(m, i);
        return true;
      }
      else {
        val(list(*m)[i]) = val(e);
        return true;
      }
    }
  }
  
  if (val(e) != zero(*m)) {
    if (nelem(*m) < MAX_N_ELEM) {
      max(*m) = max_pos(max(*m), pos(e));
      min(*m) = min_pos(min(*m), pos(e));
      list(*m)[nelem(*m)++] = e;
    }
    else {
      return false;
    }
  }

  return true;
}


/*
 * input: ptr to sparse matrix, index for the list in the matrix
 *
 * removes element from a position in the list
 *
 * note: doesn't make any verifications on what was on the position
 */
static void remove_at(sparse *m, unsigned long index)
{
  int i;

  /* if the position is greater than the number of elements, return
   * alternatively, if nelem(m) == 0, then the expression is true,
   * which deals with the empty matrix case */
  if (index >= nelem(*m)) return;

  nelem(*m)--;
  for (i = index; i < nelem(*m); i++) {
    list(*m)[i] = list(*m)[i + 1];
  }

  update_max_min(m);
}

/*
 * input: ptr to sparse matrix
 *
 * updates the max and min of a sparse 
 *
 * may be necessary, since when removing there is no way of knowing
 * if the element was the one defining the max or min
 */
static void update_max_min(sparse *m)
{
  int i;
  min(*m) = init_pos(ULONG_MAX, ULONG_MAX);
  max(*m) = init_pos(0, 0);

  for (i = 0; i < nelem(*m); i++) {
    max(*m) = max_pos(max(*m), pos(list(*m)[i]));
    min(*m) = min_pos(min(*m), pos(list(*m)[i]));
  }
}


/*
 * inputs:
 *   ptr to sparse matrix
 *   vector (of len <len>) of indices to be removed 
 *
 * removes elements from several positions in the
 * ordered list indices[]
 * updates the matrix (list of elements, max, min, and number of elements)
 */
static void remove_mult(sparse *m, int indices[], int len)
{
  int i, j, k;
  if (len == 0) return;

  min(*m) = init_pos(ULONG_MAX, ULONG_MAX);
  max(*m) = init_pos(0, 0);

  for (i = j = k = 0; i < nelem(*m); i++) {
    if (k >= len || i != indices[k]) {
      list(*m)[j] = list(*m)[i];
      max(*m) = max_pos(max(*m), pos(list(*m)[j]));
      min(*m) = min_pos(min(*m), pos(list(*m)[j]));
      j++;
    }
    else {
      k++;
    }
  }

  nelem(*m) = j;
}


/*
 * input: sparse matrix
 *
 * output: all the elements on a sparse matrix
 *
 * if the matrix is empty, print "empty matrix"
 */
void print_sparse(sparse m)
{
  char str[BUFFER_OUT_EL];
  int i;

  if (empty_sparse(m)) {
    printf("empty matrix\n");
    return;
  }

  for (i = 0; i < nelem(m); i++) {
    out_el(list(m)[i], str);
    printf("%s\n", str); 
  }
}


/*
 * input: sparse matrix
 *
 * output: carachteristics of a sparse matrix
 *
 * if the matrix is empty, print "empty matrix"
 */
void print_charact_sparse(sparse m)
{
  if (empty_sparse(m)) {
    printf("empty matrix\n");
    return;
  }

  printf("[%lu %lu] [%lu %lu] %lu / %lu = %.3f%%\n",
      row(min(m)), col(min(m)), row(max(m)), col(max(m)),
      nelem(m), size_sparse(m), 100 * density_sparse(m));
}


/*
 * inputs: ptr to matrix, new value of zero
 *
 * changes the value of zero
 *
 * removes all occurrences of the new zero in the list
 */
void change_zero(sparse *m, double new_zero)
{
  int indices[MAX_N_ELEM];
  int i, j;

  /* if the new zero is the old one or if the matrix is empty,
   * there is nothing to be done */
  if (new_zero == zero(*m) || empty_sparse(*m)) return;

  zero(*m) = new_zero;
  for (i = j = 0; i < nelem(*m); i++) {
    if (val(list(*m)[i]) == zero(*m)) {
      indices[j++] = i;
    }
  }

  remove_mult(m, indices, j);
}


/* 
 * inputs: sparse matrix, index of the row to be printed
 *
 * output: row, represented by its values
 */
void print_row_i(sparse m, unsigned long row_i)
{
  int index[width_sparse(m)];
  unsigned long i;
  double val; 
  bool emptyrow = true;

  /* first check for an empty row:
   * empty_sparse or row index out of bounds */
  if (empty_sparse(m) || row_i < row(min(m)) || row_i > row(max(m))) {
    printf("empty line\n");
    return;
  }

  /* initialize index at zero to distinguish from 0th row */
  for (i = 0; i < width_sparse(m); index[i++] = -1);

  for (i = 0; i < nelem(m); i++) {
    if (row(pos(list(m)[i])) == row_i) {
      index[col(pos(list(m)[i])) - col(min(m))] = i;
      emptyrow = false;
    }
  }

  if (emptyrow) {
    printf("empty line\n");
    return;
  }

  for (i = 0; i < width_sparse(m); i++) {
    if (index[i] == -1) {
      val = zero(m);
    }
    else {
      val = val(list(m)[index[i]]);
    }

    printf(" %.3f", val);
  }
  printf("\n");
}

/* 
 * inputs: sparse matrix, index of the column to be printed
 *
 * output: column, represented by its elements, printed by out_el
 */
void print_col_j(sparse m, unsigned long col_j)
{
  char str[BUFFER_OUT_EL];
  int index[height_sparse(m)];
  unsigned long i;
  el e; 
  bool emptycol = true;

  /* first check for an empty column:
   * empty_sparse or column index out of bounds */
  if (empty_sparse(m) || col_j < col(min(m)) || col_j > col(max(m))) {
    printf("empty column\n");
    return;
  }

  /* initialize index at zero to distinguish from 0th row */
  for (i = 0; i < height_sparse(m); index[i++] = -1);

  for (i = 0; i < nelem(m); i++) {
    if (col(pos(list(m)[i])) == col_j) {
      index[row(pos(list(m)[i])) - row(min(m))] = i;
      emptycol = false;
    }
  }

  if (emptycol) {
    printf("empty column\n");
    return;
  }

  for (i = 0; i < height_sparse(m); i++) {
    if (index[i] == -1) {
      e = init_el(zero(m), init_pos(i + row(min(m)), col_j));
    }
    else {
      e = list(m)[index[i]];
    }

    out_el(e, str);
    printf("%s\n", str);
  }
}


/* 
 * inputs: sparse matrix
 *
 * prints all els over principal diagonal
 */
void print_all_over_diag(sparse m)
{
  size_t i;
  for (i = 0; i < nelem(m); i++) {
    if (col(pos(list(m)[i])) > row(pos(list(m)[i]))) {
      printf("%lf\n", val(list(m)[i]));
    }
  }
}
    

/*
 * inputs: sparse matrix
 *
 * prints the number of different values on the matrix
 */
void print_n_diff_vals(sparse m)
{
  double min_val;
  bool hash[MAX_VAL_DIFF];
  size_t i;
  size_t count;

  /* if there are no elements, don't print anything */
  if (nelem(m) == 0) 
    return;

  for (i = 0; i < MAX_VAL_DIFF; hash[i++] = false);

  min_val = val(list(m)[0]);
  for (i = 1; i < nelem(m); i++) {
    min_val = (val(list(m)[i]) < min_val) ? val(list(m)[i]) : min_val;
  }

  count = 0;
  for (i = 0; i < nelem(m); i++) {
    if (hash[(unsigned long) val(list(m)[i])] == false) {
      count++;
      hash[(unsigned long) val(list(m)[i])] = true;
    }
  }

  printf("%lu\n", count);
}
