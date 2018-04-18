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
#include <stdio.h>
#include <stdlib.h>

/* constructor */
pos init_pos(unsigned int i, unsigned int j)
{
  pos p;
  p.i = i;
  p.j = j;
  return p;
}

/* output string */
char *out_pos(pos p)
{
  char *str = (char *) malloc(MAX_BUFFER_OUT_POS * sizeof(char));
  if (snprintf(str, MAX_BUFFER_OUT_POS, "[%u;%u]", line(p), col(p)) >= MAX_BUFFER_OUT_POS) {
    perror("pos.c: out_pos: buffer overflow trying to output pos.\n");
    printf("pos.c: out_pos: buffer overflow trying to output pos.\n");
    exit(1);
  }
  return str;
}
