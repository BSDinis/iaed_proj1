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

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

/* compresses a matrix into 3 arrays */
static unsigned compress(sparse m, double vals[], unsigned rows[], unsigned offsets[]);

/* fills two arrays:
 *   1) list (a 2d array): in each position d, has the indices of the 
 *                         rows whose density is d
 *   2) size: has the number of rows whose density is d
 */
static unsigned list_rows_by_density(sparse m, unsigned list[width_sparse(m)][height_sparse(m)], unsigned size[]);

/* calculates the density of the ith row of a sparse matrix */
static unsigned row_density(sparse m, unsigned i);

/* 
 * finds a slot for the ith line of the sparse matrix in the compressed vector;
 * fills in both the values and the line vector
 * returns the offset
 */
static unsigned find_slot(sparse m, double vals[], unsigned rows[], unsigned vals_len,  unsigned i);


/*
 * given two arrays and an offset (applied to the second),
 * finds if they overlap or not
 * true: they do overlap
 * false: they don't overlap
 * (note: the zero(m) is not considered a filled position)
 */
static bool overlap(double a[], double b[],
    unsigned len_a, unsigned len_b, unsigned offset, double zero);

/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/
/* compresses a sparse matrix, printing the output */
void compress_sparse(sparse m)
{
  double vals[nelem(m) * 2];
  unsigned rows[nelem(m) * 2];
  unsigned offsets[height_sparse(m)];
  unsigned compressed_len;
  unsigned i;

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
  for (i = 0; i < compressed_len; i++) printf(" %u", rows[i]);
  printf("\n");

  printf("offset =");
  for (i = 0; i <= height_sparse(m); i++) printf(" %u", offsets[i]);
  printf("\n");
}

/* auxiliar function: applies the actual compression algorithm 
 * returns the length of the compressed vectors (vals and rows)*/
static unsigned compress(sparse m, double vals[], unsigned rows[], unsigned offsets[])
{
  unsigned list[width_sparse(m)][height_sparse(m)];
  unsigned size[width_sparse(m)];
  unsigned max_dens;
  unsigned compressed_len, max_offset;
  int i, j, row;

  compressed_len = 0;
  for (i = 0; i < 2 * nelem(m); i++) {
    vals[i] = zero(m);
    rows[i] = 0;
  }

  max_offset = 0;
  max_dens = list_rows_by_density(m, list, size);
  for (i = max_dens; i >= 0; i--) {
    for (j = 0; j < size[i]; j++) {
      row = list[i][j];
      offsets[row - row(min(m))] = find_slot(m, vals, rows, compressed_len, row);
      max_offset = max_int(max_offset, offsets[row - row(min(m))]);
      compressed_len = max_offset + width_sparse(m);
    }
  }

  return compressed_len;
}


/* 
 * fills two arrays:
 *   1) list (a 2d array): in each position d, has the indices of the 
 *                         rows whose density is d
 *   2) size: has the number of rows whose density is d
 *
 * returns the maximum density;
 */
static unsigned list_rows_by_density(sparse m, unsigned list[width_sparse(m)][height_sparse(m)], unsigned size[])
{
  unsigned max_dens = 0, dens;
  int i;

  /* initialize the size array */
  for (i = 0; i < width_sparse(m); size[i++] = 0);

  /* initialize the list array */
  for (i = row(min(m)); i <= row(max(m)); i++) {
    dens = row_density(m, i);
    max_dens = max_int(max_dens, dens);
    list[dens][size[dens]++] = i;
  }

  return max_dens;
}

/* calculates the density of the ith row of a sparse matrix */
static unsigned row_density(sparse m, unsigned i)
{
  unsigned cnt = 0;
  unsigned j = 0;
  while(j != nelem(m)) {
    if (row(pos(list(m)[j++])) == i) {
      cnt++;
    }
  }

  return cnt;
}


/* 
 * finds a slot for the ith row of the sparse matrix in the compressed vector;
 * fills in both the values and the row vector
 * returns the offset
 */
static unsigned find_slot(sparse m, double vals[], unsigned rows[], unsigned vals_len,  unsigned i)
{
  double row[width_sparse(m)];
  unsigned j, offset;
  bool empty_row = true;


  for (j = 0; j < width_sparse(m); row[j++] = zero(m));

  for (j = 0; j < nelem(m); j++) {
    if (row(pos(list(m)[j])) == i) {
      row[col(pos(list(m)[j])) - col(min(m))] = val(list(m)[j]);
      empty_row = false;
    }
  }

  /* in case the row is empty, there is nothing to be done and the offset is 0 */
  if (empty_row) return 0;

  for (offset = 0; overlap(vals, row, vals_len, width_sparse(m), offset, zero(m)) 
                && vals_len - offset > 0; offset++);


  for (j = 0; j < width_sparse(m); j++) {
    if (row[j] != zero(m)) {
      vals[offset + j] = row[j];
      rows[offset + j] = i;
    }
  }

  return offset;
}


/*
 * given two arrays and an offset (applied to the second),
 * finds if they overlap or not
 * true: they do overlap
 * false: they don't overlap
 * (note: the zero is not considered a filled position)
 */
static bool overlap(double a[], double b[], unsigned len_a, unsigned len_b, unsigned offset, double zero)
{
  unsigned len = min_int(len_a - offset, len_b);
  unsigned i;
  for (i = 0; i < len; i++) {
    if (a[offset + i] != zero && b[i] != zero) {
      return true;
    }
  }

  return false;
}
