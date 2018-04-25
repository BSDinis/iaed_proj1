/*
 *
 * compress.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * define compression capability
 */

#include "compress.h"
#include "pos.h"
#include "el.h"

#include <stdio.h>

#define max_int(a, b) ((a > b) ? a : b)
#define min_int(a, b) ((a < b) ? a : b)

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

static unsigned long compress(sparse m, double vals[], unsigned long rows[], unsigned long offsets[]);

static void list_rows_by_density(sparse m, unsigned long rows[min_int(height_sparse(m), nelem(m)) + 1]);

static unsigned long find_slot(sparse m, double vals[], unsigned long rows[], unsigned long vals_len,  unsigned long i);

static bool overlap(double a[], el b[], unsigned len_b, unsigned long offset, unsigned long left_col, double zero);

/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/


/* 
 * input: sparse matrix
 * prints the compressed matrix, given by 3 vectors
 */
void compress_sparse(sparse m)
{
  double vals[nelem(m) * 2];
  unsigned long rows[nelem(m) * 2];
  unsigned long offsets[height_sparse(m)];
  unsigned long compressed_len;
  unsigned long i;

  if (density_sparse(m) > 0.5) {
    printf("dense matrix\n");
    return;
  }

  compressed_len = compress(m, vals, rows, offsets);

  printf("value =");
  for (i = 0; i < compressed_len; i++) printf(" %.3f", vals[i]);
  printf("\n");
  
  printf("index =");
  for (i = 0; i < compressed_len; i++) printf(" %lu", rows[i]);
  printf("\n");

  printf("offset =");
  for (i = 0; i < height_sparse(m); i++) printf(" %lu", offsets[i]);
  printf("\n");
}


/* 
 * input: sparse matrix; 3 vectors to be filled
 * vals: has the values
 * rows: has the row number corresponding to the val 
 * (ie, the value in val[i] is from the row rows[i])
 * offsets: how much each row was displaced to fit in the vals vector
 *
 * each row is completely represented (there may be padding)
 * so the length of the vals and rows vector is max_offset + width
 *
 * gaps (ie: free positions) in the first two elemnts are of the form:
 * vals[i] = zero(m) && rows[i] = 0
 *
 * empty rows have offset = 0;
 */
static unsigned long compress(sparse m, double vals[], 
    unsigned long rows[], unsigned long offsets[])
{
  unsigned long row_dens[height_sparse(m)];
  unsigned long compressed_len, max_offset;
  int i, row;
  
  compressed_len = 0;
  for (i = 0; i < 2 * nelem(m); i++) {
    vals[i] = zero(m);
    rows[i] = 0;
  }

  max_offset = 0;

  /* fills the row_dens vector with row indices by the order in which they
   * should be inserted in the compressed vectors */
  list_rows_by_density(m, row_dens);

  for (i = 0; i < height_sparse(m); i++) {
    row = row_dens[i];
    offsets[row - row(min(m))] = find_slot(m, vals, rows, compressed_len, row);
    max_offset = max_int(max_offset, offsets[row - row(min(m))]);
    compressed_len = max_offset + width_sparse(m);
  }

  return compressed_len;
}


/* 
 * input: sparse matrix; row_dens, the vector to be filled
 *
 * row_dens is filled with indices of rows in m
 * they appear by the following order:
 *
 * if row <i> has greater density (number of elements) than row <j>
 * then row <i> appears first
 * 
 * if row <i> and row <j> have the same density and i < j,
 * then row <i> appears first
 */
static void list_rows_by_density(sparse m, unsigned long row_dens[height_sparse(m)])
{
  unsigned long max_size = height_sparse(m);
  unsigned long dens[max_size];
  unsigned long max_dens_row, max_dens;
  int i, j;

  /* initialize at 1: zero is for checked out lines */
  for (i = 0; i < max_size; dens[i++] = 1);

  for (i = 0; i < nelem(m); i++) {
    dens[row(pos(list(m)[i])) - row(min(m))]++;
  }

  /* keeps choosing the maximum density row */
  for (i = 0; i < max_size; i++) {
    max_dens = max_dens_row = 0;
    for (j = 0; j < max_size; j++) {
      if ((int) dens[j] > max_dens) {
        max_dens = dens[j];
        max_dens_row = j;
      }
    }
    dens[max_dens_row] = 0;
    row_dens[i] = max_dens_row + row(min(m));
  }
}


/* 
 * inputs: 
 *   sparse matrix,
 *   vals list,
 *   rows list,
 *   length of the compressed vector,
 *   row to be inserted.
 *
 * finds a slot for the ith row of the sparse matrix in the compressed vector;
 * fills in both the values and the row vector
 *
 * return: the offset for the ith row
 */
static unsigned long find_slot(sparse m, double vals[], unsigned long rows[], unsigned long vals_len,  unsigned long i)
{
  el row[width_sparse(m)];
  unsigned long j, k, offset;

  for (j = 0, k = 0; j < nelem(m); j++) {
    if (row(pos(list(m)[j])) == i) {
      row[k++] = list(m)[j];
    }
  }

  /* in case the row is empty, there is nothing to be done and the offset is 0 */
  if (k == 0) return 0;

  for (offset = 0; 
      overlap(vals, row, k, offset, col(min(m)), zero(m)) 
      && vals_len - offset > 0;
      offset++);


  for (j = 0; j < k; j++) {
    vals[offset + col(pos(row[j])) - col(min(m))] = val(row[j]);
    rows[offset + col(pos(row[j])) - col(min(m))] = i;
  }

  return offset;
}


/*
 * inputs:
 *   compressed array of values: <a>,
 *   array of elements: <b>; size is <len_b>
 *   offset to be tested,
 *   leftmost column (necessary to normalize the columns of the elements)
 *   value of zero.
 *
 * checks if the offset, when applied to b, makes it fit.
 *
 * returns:
 *   true: they do overlap
 *   false: they don't overlap
 *
 * (note: the zero is not considered a filled position)
 */
static bool overlap(double a[], el b[], unsigned len_b, unsigned long offset, unsigned long left_col, double zero)
{
  unsigned long i;
  for (i = 0; i < len_b; i++) {
    if (a[offset + col(pos(b[i])) - left_col] != zero) {
      return true;
    }
  }

  return false;
}
