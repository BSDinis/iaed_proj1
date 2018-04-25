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
 * output string to print 
 * given an element, writes the representation on out
 */
void out_el(el e, char out[])
{
  char pos_str[BUFFER_OUT_POS];
  out_pos(pos(e), pos_str);
  sprintf(out, "%s=%.3f", pos_str, val(e));
}

/* 
 * output string to save to file 
 * given an element, writes the representation on out
 */
void save_el(el e, char out[])
{
  sprintf(out, "%lu %lu %f", row(pos(e)), col(pos(e)), val(e));
}


/* 
 * get element from string 
 * the string has the format: <ulong> <ulong> <double>
 */
el str_to_el(char *input)
{
  el e;
  sscanf(input, "%lu %lu %lf", &row(pos(e)), &col(pos(e)), &val(e));
  return e;
}
