/*
 *
 * el.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the element datatype
 */


#include "el.h"
#include <stdio.h>
#include <ctype.h>


/* constructor */
el init_el(double val, pos p)
{
  el e;
  val(e) = val;
  pos(e) = p;
  return e;
}


/* output string to print */
void out_el(el e, char out[])
{
  char pos_str[BUFFER_OUT_POS];
  out_pos(pos(e), pos_str);
  sprintf(out, "%s=%.3f", pos_str, val(e));
}

/* output string to save to file */
void save_el(el e, char out[])
{
  sprintf(out, "%u %u %f", row(pos(e)), col(pos(e)), val(e));
}


/* get element from string */
el str_to_el(char *input)
{
  el e;
  sscanf(input, "%u %u %lf", &row(pos(e)), &col(pos(e)), &val(e));
  return e;
}
