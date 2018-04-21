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

/* gets the first key: row of the position 
unsigned key1(item a)
{
  return row(pos(a));
}
*/

/* gets the second key: col of the position 
unsigned key2(item a)
{
  return col(pos(a));
}
*/

/* compares using the first key as the most significant one 
bool less1(item a, item b)
{
  return ((key1(a) < key1(b)) ||
      ((key1(a) == key1(b)) && key2(a) < key2(b)));
}
*/

/* compares using the second key as the most significant one
bool less2(item a, item b)
{
  return ((key2(a) < key2(b)) ||
      ((key2(a) == key2(b)) && key1(a) < key1(b)));
}
*/

/* 
 * implements the counting sort using a certain compare function
 * 
 * l: leftmost position to consider
 * r: rightmost position to consider
 * m: minimum value of the key of the items of the list
 * M: maximum value of the key of the items of the list
 * key: returns the key to be considered
 */
void counting_sort(item list[], int l, int r, int m, int M, unsigned (*key)(item))
{
  /* size_cnt is the number of possible keys 
   * size_aux is the number of elements in list between l and r*/
  int i;
  int size_cnt = M - m + 1, size_aux = r - l + 1;
  int cnt[size_aux];
  item aux[size_aux];
  
  for (i = 0; i < size_cnt; cnt[i++] = 0);

  /* count; -m shifts input from at [m, M] to [0, M - m] */
  for (i = l; i <= r; i++) {
    cnt[(*key)(list[i]) - m + 1]++; 
  }

  /* accumulate */
  for (i = 1; i < size_cnt; i++) {
    cnt[i] += cnt[i - 1];
  }

  /* construct auxiliar */
  for (i = l; i <= r; i++) {
    aux[cnt[(*key)(list[i]) - m]++] = list[i];
  }

  /* copy to list */
  for (i = l; i <= r; i++) {
    list[i] = aux[i - l];
  }
}
