/*
 *
 * sort.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * header file for sort.c
 *
 * defines sorting functions
 *
 */

#ifndef SORT_H
#define SORT_H

#include <stdlib.h>
#include <stdbool.h>
#include "el.h"
#include "pos.h"
#include "sparse.h"

typedef el item;

#define swap(a, b) {item tmp = a; a = b; b = tmp;}

/* gets the first key: row of the position */
unsigned key_row(item a); 

/* gets the second key: col of the position */
unsigned key_col(item a); 

/* implements counting sort using a certain key function */
void counting_sort(item list[], int l, int r, unsigned m, unsigned M, unsigned (*key)(item));

/* implements radix sort LSD using a n key functions, 
 * given in a list of function pointers, with ascending significance
 *
 * eg: order a list of pairs of integers (x, y), with y being the 
 * least significant
 *
 * radix_sort(list, l, r, m_list, M_list, 
 * {<function that selects y>, <function that selects, x>, 2)
 */
void radix_sort(item list[], int l, int r, unsigned m_list[],
    unsigned M_list[], unsigned (*key_arr[])(item), int n);

#endif /* !SORT_H */
