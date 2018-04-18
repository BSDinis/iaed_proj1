/*
 *
 * el.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * element datatype header file
 *
 * el: the element of the sparse matrix
 * has a position [pos] and a value [double]
 *
 */

#pragma once

#include "pos.h"

// TODO: search for a more precise value, in conformity with specs
/* buffer for output string */
#define BUFFER_OUT_EL (BUFFER_OUT_POS * 2 + 10000)

typedef struct {
  double val;
  pos p;
} el;

/* selectors */
#define pos(a) (a.p)
#define val(a) (a.val)

/* equality */
#define eq_el(a, b) (pos(a) == pos(b) && val(a) == val(b))

/* comparison: operates on the value */
#define less_l(a, b) (val(a) < val(b))

/* constructor */
el init_el(double val, pos p);

/* output string: exits with error if the buffer is exceeded */
char *out_el(el e);

/* valid string representation of an element */
bool valid_el(char *str);

/* input from string */
el str_to_el(char *input);
