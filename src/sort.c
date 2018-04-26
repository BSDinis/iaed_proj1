/*
 *
 * sort.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines sorting functions
 *
 */

#include "sort.h"
#include "el.h"

#include <stdlib.h> 

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

static unsigned long key_row(el a); 

static unsigned long key_col(el a); 

static void counting_sort(el list[], int l, int r, unsigned long m,
    unsigned long M, unsigned long (*key)(el));

static void insertion_sort(el list[], int l, int r, unsigned long (*key)(el));

static void sort_list(el list[], int l, int r, unsigned long m, 
    unsigned long M, unsigned long (*key)(el));

static void radix_sort(el list[], int l, int r, unsigned long m_list[],
    unsigned long M_list[], unsigned long (*key_arr[])(el), int arglen);

/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/

/* 
 * input: element
 * returns: row of the element
 */
static unsigned long key_row(el a)
{
  return row(pos(a));
}

/* 
 * input: element
 * returns: column of the element
 */
static unsigned long key_col(el a)
{
  return col(pos(a));
}


/* 
 * implements the counting sort using a certain compare function
 * 
 * inputs:
 * list: becomes ordered 
 * l: leftmost position to consider
 * r: rightmost position to consider
 * m: minimum value of the key of the els of the list
 * M: maximum value of the key of the els of the list
 * key: returns the key to be considered
 */
static void counting_sort(el list[], int l, int r, unsigned long m, 
    unsigned long M, unsigned long (*key)(el))
{
  /* size_cnt is the number of possible keys */
  int i;
  int size_cnt = M - m + 1;
  int cnt[size_cnt + 1];
  el aux[r - l + 1];
  
  for (i = 0; i < size_cnt; cnt[i++] = 0);

  /* count; -m shifts input from at [m, M] to [0, M - m] */
  for (i = l; i <= r; i++) {
    cnt[(*key)(list[i]) - m + 1]++; 
  }

  for (i = 1; i <= size_cnt; i++) {
    cnt[i] += cnt[i - 1];
  }

  /* construct auxiliar; does the same shift */
  for (i = l; i <= r; i++) {
    aux[cnt[(*key)(list[i]) - m]++] = list[i];
  }

  /* copy to list */
  for (i = l; i <= r; i++) {
    list[i] = aux[i - l];
  }
}


/*
 * input:
 *   list: element list to be sorted (becomes ordered)
 *   l: leftmost position
 *   r: rightmost position
 *   key: returns the key for an element
 */
static void insertion_sort(el list[], int l, int r, unsigned long (*key)(el))
{
  int i, j;
  el v;

  for (i = l + 1; i <= r; i++) {
    v = list[i];
    j = i - 1;

    while (j >= l && (*key)(v) < (*key)(list[j])) {
      list[j + 1] = list[j];
      j--;
    }

    list[j + 1] = v;
  }
}


/*
 * sorts a list of elements, using either a counting sort or an insertion
 * sort, depending on the conditions given.
 *
 * inputs:
 * list: becomes ordered 
 * l: leftmost position to consider
 * r: rightmost position to consider
 * m: minimum value of the key of the els of the list
 * M: maximum value of the key of the els of the list
 * key: returns the key to be considered
 */
static void sort_list(el list[], int l, int r, unsigned long m, 
    unsigned long M, unsigned long (*key)(el))
{
  unsigned long a = r - l + 1, b = M - m + 1;
  /* counting is O(a + b) and insertion is O(a * a) */
  if (b < a * a) {
    counting_sort(list, l, r, m, M, key);
  }
  else {
    insertion_sort(list, l, r, key);
  }
}



/* 
 * implements radix sort LSD using n key functions, 
 * given in a list of function pointers, with ascending significance
 *
 * inputs:
 * list: becomes ordered 
 * l: leftmost position
 * r: rightmost position
 * m_list: list of minimum values
 * M_list: list of maximum values
 * key_arr: array of pointers to function that compute the keys
 * n: number of keys to consider
 *
 * usage example: 
 * order a list of pairs of integers (x, y), 
 * with y being the least significant
 *
 * radix_sort(list, l, r, m_list, M_list, 
 * {<function that selects y>, <function that selects, x>, 2)
 *
 */
static void radix_sort(el list[], int l, int r, unsigned long m_list[], 
    unsigned long M_list[], unsigned long (*key_arr[])(el), int arglen)
{
  int i;
  for (i = 0; i < arglen; i++) {
    sort_list(list, l, r, m_list[i], M_list[i], key_arr[i]);
  }
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
  unsigned long m_list[2], M_list[2];
  unsigned long (*key_arr[2])(el);

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
