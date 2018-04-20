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


/* output string to print*/
void out_el(el e, char *out)
{
  if (snprintf(out, BUFFER_OUT_EL, "%s=%.3lf", out_pos(pos(e)), val(e)) >= BUFFER_OUT_EL) {
    perror("el.c: out_el: buffer overflow trying to output el.\n");
    printf("el.c: out_el: buffer overflow trying to output el.\n");
    exit(1);
  }
}

/* output string to save to file*/
void save_el(el e, char *out)
{
  if (snprintf(out, BUFFER_OUT_EL, "%u %u %lf", row(pos(e)), col(pos(e)), val(e)) >= BUFFER_OUT_EL) {
    perror("el.c: save_el: buffer overflow trying to output el.\n");
    printf("el.c: save_el: buffer overflow trying to output el.\n");
    exit(1);
  }
}
