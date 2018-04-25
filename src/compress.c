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

/* compresses a matrix into 3 arrays */
static unsigned long compress(sparse m, double vals[], unsigned long rows[], unsigned long offsets[]);

/* fills the rows array which has the order by which the compressed array should be filled*/
static void list_rows_by_density(sparse m, unsigned long rows[min_int(height_sparse(m), nelem(m)) + 1]);

/* 
 * finds a slot for the ith line of the sparse matrix in the compressed vector;
 * fills in both the values and the line vector
 * returns the offset
 */
static unsigned long find_slot(sparse m, double vals[], unsigned long rows[], unsigned long vals_len,  unsigned long i);


/*
 * given two arrays and an offset (applied to the second),
 * finds if they overlap or not
 * true: they do overlap
 * false: they don't overlap
 * (note: the zero(m) is not considered a filled position)
 */
static bool overlap(double a[], el b[], unsigned long len_b, unsigned long offset, unsigned long left_col, double zero);

/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/
/* compresses a sparse matrix, printing the output */
void compress_sparse(sparse m)
{
  double vals[nelem(m) * 2];
  unsigned long rows[nelem(m) * 2];
  unsigned long offsets[nelem(m)];
  unsigned long compressed_len;
  unsigned long i;

  /* check for dense matrices */
  if (density_sparse(m) > 0.5) {
    printf("dense matrix\n");
    return;
  }


  /* do the compression */
  compressed_len = compress(m, vals, rows, offsets);

  /* print the result */
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

/* auxiliar function: applies the actual compression algorithm 
 * returns the length of the compressed vectors (vals and rows)*/
static unsigned long compress(sparse m, double vals[], unsigned long rows[], unsigned long offsets[])
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
 * fills a vector
 * rows[] ordered by density (decreasing) and row (increasing)
 * ie if two rows have the same density, the one with lesser line comes first
 */
static void list_rows_by_density(sparse m, unsigned long row_dens[min_int(height_sparse(m), nelem(m)) + 1])
{
  unsigned long max_size = min_int(height_sparse(m), nelem(m)) + 1;
  unsigned long dens[max_size];
  unsigned long max_dens_row, max_dens;
  int i, j;

  /* initialize at 1: zero is for checked out lines */
  for (i = 0; i < max_size; dens[i++] = 1);

  /* initialize density vector */
  for (i = 0; i < nelem(m); i++) {
    dens[row(pos(list(m)[i])) - row(min(m))]++;
  }

  /* fill in rows vector 
   * O(n^2): you can do better
   */
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

/* calculates the density of the ith row of a sparse matrix 
static unsigned long row_density(sparse m, unsigned long i)
{
  unsigned long cnt = 0;
  unsigned long j = 0;
  while(j != nelem(m)) {
    if (row(pos(list(m)[j++])) == i) {
      cnt++;
    }
  }

  return cnt;
}
*/


/* 
 * finds a slot for the ith row of the sparse matrix in the compressed vector;
 * fills in both the values and the row vector
 * returns the offset
 */
static unsigned long find_slot(sparse m, double vals[], unsigned long rows[], unsigned long vals_len,  unsigned long i)
{
  el row[width_sparse(m) + 1];
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
 * given an array of vals and an array of elements,
 * given an offset applied to the second array
 * given the leftmost collumn of the sparse matrix 
 * (to normalize the poisitions to zero)
 *
 * finds if they overlap or not (ie, if b fits in a)
 * true: they do overlap
 * false: they don't overlap
 * (note: the zero is not considered a filled position)
 */
static bool overlap(double a[], el b[], unsigned long len_b, unsigned long offset, unsigned long left_col, double zero)
{
  unsigned long i;
  for (i = 0; i < len_b; i++) {
    if (a[offset + col(pos(b[i])) - left_col] != zero) {
      return true;
    }
  }

  return false;
}
