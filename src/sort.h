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

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include "el.h"
#include "pos.h"

typedef int item;

#define swap(a, b) {item tmp = a; a = b; b = tmp;}

/* gets the first key: row of the position */
//unsigned key1(item a); 

/* gets the second key: col of the position */
//unsigned key2(item a); 

/* compares using the first key as the most significant one */
//bool less1(item a, item b);

/* compares using the second key as the most significant one */
//bool less2(item a, item b);

/* implements the counting sort using a certain key function */
void counting_sort(item list[], int l, int r, int m, int M, unsigned (*key)(item));

