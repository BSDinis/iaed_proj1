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


/* 
 * constructor:
 * input: a value (double) and a position 
 */
el init_el(double val, pos p)
{
  el e;
  val(e) = val;
  pos(e) = p;
  return e;
}


/* 
 * input: element and a buffer to be written
 * writes the representation on out
 * format: <pos>=<value>
 */
void out_el(el e, char out[])
{
  char pos_str[BUFFER_OUT_POS];
  out_pos(pos(e), pos_str);
  sprintf(out, "%s=%.3f", pos_str, val(e));
}

/* 
 * output string to save to file 
 * input: element and a buffer to be written
 * writes the representation on out
 * format: <row> <col> <value>
 */
void save_el(el e, char out[])
{
  sprintf(out, "%lu %lu %f", row(pos(e)), col(pos(e)), val(e));
}


/*                 
 * input: string 
 * returns: element extracted from string
 * format: <ulong> <ulong> <double>
 */
el str_to_el(char *input)
{
  el e;
  sscanf(input, "%lu %lu %lf", &row(pos(e)), &col(pos(e)), &val(e));
  return e;
}
