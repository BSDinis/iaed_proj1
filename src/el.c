/*
 *
 * el.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the element datatype
 */


#include "pos.h"

/* constructor */
el init_el(double val, pos p);
{
  el e;
  e.val = val;
  e.p = p;
  return e;
}
