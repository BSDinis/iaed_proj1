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

#ifndef EL_H
#define EL_H

#include "pos.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * buffer for output string for element
 * position + greatest finite double is ~ 1.8 E 308
 * added 5 chars for delimiters 
 */
#define BUFFER_OUT_EL (BUFFER_OUT_POS + 309 + 5)

typedef struct {
  double val;
  pos p;
} el;

/* selectors */
#define pos(a) (a.p)
#define val(a) (a.val)

/* equality */
#define eq_el(a, b) (eq_pos(pos(a), pos(b)) && val(a) == val(b))

/* comparison: operates on the value */
#define less_el(a, b) (val(a) < val(b))

el init_el(double val, pos p);

void out_el(el e, char out[]);

void save_el(el e, char out[]);

el str_to_el(char *input);


#endif /* !EL_H */
