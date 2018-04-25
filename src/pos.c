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

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

/* constructor */
pos init_pos(unsigned long i, unsigned long j)
{
  pos p;
  p.i = i;
  p.j = j;
  return p;
}

/* output string */
void out_pos(pos p, char *out)
{
  sprintf(out, "[%lu;%lu]", row(p), col(p));
}


/* 
 * returns the max of the two positions
 * the max is the position in which each entry is the max of
 * each entry of the inputs
 */
pos max_pos(pos a, pos b)
{
  return init_pos(max(row(a), row(b)), max(col(a), col(b)));
}

/* 
 * returns the min of the two positions
 * the min is the position in which each entry is the min of
 * each entry of the inputs
 */
pos min_pos(pos a, pos b)
{
  return init_pos(min(row(a), row(b)), min(col(a), col(b)));
}
