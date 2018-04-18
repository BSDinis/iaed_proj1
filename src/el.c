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
#include <stdlib.h>

/* constructor */
el init_el(double val, pos p)
{
  el e;
  e.val = val;
  e.p = p;
  return e;
}

/* output string */
char *out_el(el e)
{
  char *str = (char *) malloc(BUFFER_OUT_EL * sizeof(char));
  if (snprintf(str, BUFFER_OUT_EL, "%s=%.3lf", out_pos(pos(e)), val(e)) >= BUFFER_OUT_EL) {
    perror("el.c: out_el: buffer overflow trying to output el.\n");
    printf("el.c: out_el: buffer overflow trying to output el.\n");
    exit(1);
  }
  return str;
}
