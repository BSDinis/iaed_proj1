/*
 *
 * pos.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the position datatype
 */


#include "pos.h"

/* constructor */
pos init_pos(unsigned int i, unsigned int j)
{
  pos p;
  p.i = i;
  p.j = j;
  return p;
}
