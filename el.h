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

typedef struct {
  double val;
  pos p;
} el;

/* selectors */
#define pos(a) (a.pos)
#define val(a) (a.val)

/* equality */
#define eq_el(a, b) (pos(a) == pos(b) && val(a) == val(b))

/* comparison: operates on the value */
#define less_l(a, b) (val(a) < val(b))

/* constructor */
el init_el(double val, pos p);