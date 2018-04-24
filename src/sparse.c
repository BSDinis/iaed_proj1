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
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>

#include "sort.h"
#include "sparse.h"


/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

/* creates new instance of a sparse matrix */
static sparse init_new_sparse();

/* converts a file to a sparse matrix */
static sparse file_to_sparse(char *filename);

/* removes element from a position in the list */
static void remove_at(sparse *m, unsigned index);

/* updates max and min */
static void update_max_min(sparse *m);

/* removes elements from several positions */
static void remove_mult(sparse *m, unsigned index[], unsigned len);


/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/
/* 
 * initializes a matrix
 * may take a filename as input, in which case tries to load 
 * matrix of a *.sm file; if it fails, initializes a new matrix
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

  zero(m) = 0;
  nelem(m) = 0; 
  min(m) = init_pos(0, 0);
  max(m) = init_pos(0, 0);

  return m;
}


/* 
 * converts a file to a sparse matrix
 *
 * return codes:
 *   0: sucessful coversion
 *   1: unsucessful coversion
 *  
 */
static sparse file_to_sparse(char *filename)
{
  FILE *fp;
  int i = 0;
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
  while (fgets(input, BUFFER_OUT_EL, fp) != NULL && add_el(&m, str_to_el(input))) {
    i++;
  }

  fclose(fp);
  return m;
}

/* exporting */

/*
 * converts a matrix into a file
 *
 * saves the matrix to file as it will be read:
 * the first three lines are the allocd, nelem and zero values
 * the proceding nelem lines are the elements
 *
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
 * adds a new element to the matrix
 * if the value is 'zero', removes the value in that position
 * if the position has a value, replace it.
 * may have to reallocate memory for the list
 * 
 * Return codes:
 *   true: Successful operation
 *   false: Space limit reached
 */
bool add_el(sparse *m, el e)
{
  int i;


  /* if the matrix is empty and the value is not zero, 
   * initialize the min and max */
  if (empty_sparse(*m) && val(e) != zero(*m)) {
    min(*m) = max(*m) = pos(e);
    list(*m)[nelem(*m)++] = e;
    return true;
  }

  /* check if there is an element with the same position in the matrix
   *
   * if there is, replace or remove and exit.
   * important property: there aren't repeated positions
   *
   * quick check */
  if (row(pos(e)) >= row(min(*m)) && row(pos(e)) <= row(max(*m)) &&
        col(pos(e)) >= col(min(*m)) && col(pos(e)) <= col(max(*m))) {

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
 * removes element from a position in the list
 *
 * note: doesn't make any verifications on what was on the position
 */
static void remove_at(sparse *m, unsigned index)
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
 * updates max and min
 *
 * may be necessary, since when removing there is no way of knowing
 * if the element was the one defining the max or min
 */
static void update_max_min(sparse *m)
{
  int i;
  pos nmax, nmin;

  if (empty_sparse(*m)) {
    max(*m) = min(*m) = init_pos(0, 0);
    return;
  }

  nmax = nmin = pos(list(*m)[0]);
  for (i = 0; i < nelem(*m); i++) {
    nmax = max_pos(nmax, pos(list(*m)[i]));
    nmin = min_pos(nmin, pos(list(*m)[i]));
  }
  
  max(*m) = nmax;
  min(*m) = nmin;
}


/*
 * removes elements from several positions in the
 * ordered list indices[]
 */
static void remove_mult(sparse *m, unsigned indices[], unsigned len)
{
  pos nmin = init_pos(UINT_MAX, UINT_MAX), nmax = init_pos(0, 0);
  int i, j, k;

  if (len == 0) return;

  for (i = j = k = 0; i < nelem(*m); i++) {
    if (k >= len || i != indices[k]) {
      list(*m)[j] = list(*m)[i];
      nmax = max_pos(nmax, pos(list(*m)[j]));
      nmin = min_pos(nmin, pos(list(*m)[j]));
      j++;
    }
    else {
      k++;
    }
  }

  max(*m) = nmax;
  min(*m) = nmin;
  nelem(*m) = j;
}


/*
 * prints all the elements on a sparse matrix
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
 * prints the carachteristics of a sparse matrix
 */
void print_charact_sparse(sparse m)
{
  if (empty_sparse(m)) {
    printf("empty matrix\n");
    return;
  }

  printf("[%u %u] [%u %u] %u / %u = %.3f%%\n",
      row(min(m)), col(min(m)), row(max(m)), col(max(m)),
      nelem(m), size_sparse(m), 100 * density_sparse(m));
}


/*
 * changes the value of zero
 * removes all occurrences of zero in the list
 */
void change_zero(sparse *m, double new_zero)
{
  unsigned indices[MAX_N_ELEM];
  unsigned i, j;

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
 * sorts a sparse matrix, with regard to either the columns or the rows,
 * depending on a flag
 */
void sort_sparse(sparse *m, bool col)
{
  /* creates instances for arguments of the sorting function */
  /* magic number 2: dimension of the tensor (matrix -> rank 2 tensor)
   * is not a constant as it would be more confusing and would not have
   * any purpose, since we need to assign values to the lists in each
   * position */
  unsigned m_list[2], M_list[2];
  unsigned (*key_arr[2])(item);

  /* variables a and b avoid code repetition */
  int a, b;
  if (!col) {
    /* order by rows */
    a = 0;
    b = 1;
  }
  else {
    /* order by columns */
    a = 1;
    b = 0;
  }

  m_list[a] = col(min(*m));
  M_list[a] = col(max(*m));
  key_arr[a] = key_col;
  m_list[b] = row(min(*m));
  M_list[b] = row(max(*m));
  key_arr[b] = key_row;

  radix_sort(list(*m), 0, nelem(*m) - 1, m_list, M_list, key_arr, 2);
}


/* print a row */
void print_row_i(sparse m, unsigned row_i)
{
  el list[MAX_N_ELEM];
  unsigned i, j, k;

  if (row_i < row(min(m)) || row_i > row(max(m))) {
    printf("empty line\n");
    return;
  }

  for (i = j = 0; i < nelem(m); i++) {
    if (row(pos(list(m)[i])) == row_i) {
      list[j++] = list(m)[i];
    }
  }
  if (j == 0) {
    printf("empty line\n");
    return;
  }
  j--;

  counting_sort(list, 0, j, col(min(m)), col(max(m)), &key_col);

  for (i = col(min(m)), k = 0; i <= col(max(m)); i++) {
    if (k <= j && col(pos(list[k])) == i) {
      printf(" %.3f", val(list[k++]));
    }
    else {
      printf(" %.3f", zero(m));
    }
  }
  printf("\n");
}

/* print a col */
void print_col_j(sparse m, unsigned col_j)
{
  unsigned i, j, k;
  el list[MAX_N_ELEM];
  char str[BUFFER_OUT_EL];

  if (col_j < col(min(m)) || col_j > col(max(m))) {
    printf("empty column\n");
    return;
  }

  for (i = j = 0; i < nelem(m); i++) {
    if (col(pos(list(m)[i])) == col_j) {
      list[j++] = list(m)[i];
    }
  }
  if (j == 0) {
    printf("empty column\n");
    return;
  }
  j--;

  counting_sort(list, 0, j, row(min(m)), row(max(m)), &key_row);

  for (i = row(min(m)), k = 0; i <= row(max(m)); i++) {
    if (k <= j && row(pos(list[k])) == i) {
      out_el(list[k++], str);
    }
    else {
      out_el(init_el(0, init_pos(i, col_j)), str);
    }
    printf("%s\n", str);
  }
}
