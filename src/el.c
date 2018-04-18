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
#include <ctype.h>


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


/* 
 * valid string representation of an element
 *
 * checks if a string matches a representation of an element:
 * [%u;%u]=%lf
 */
bool valid_el(char *str)
{
  int i = 0;
  if (str[i++] != '[') return false;

  while (str[i] != '=' && str[i] != '\0') {
    if (!isdigit(str[i++])) return false; 
  }
  if (str[i] == '\0' || str[i] != ';') return false;
  i++;

  while (str[i] != ']' && str[i] != '\0') {
    if (!isdigit(str[i++])) return false;
  }
  if (str[i] == '\0' || str[i] != ']') return false;
  i++;
  if (str[i] == '\0' || str[i] != '=') return false;
  i++;

  while(str[i] != '\0' && str[i] != '.') {
    if (!isdigit(str[i++])) return false;
  }

  if (str[i++] == '\0') return true;

  while(str[i] !=  '\0') {
    if (!isdigit(str[i++])) return false;
  }

  return true;
}


/* 
 * input from string 
 *
 * given a string: <pos>=<val>
 * extracts an element
 *
 * does not accomodate for invalid strings,
 * a sanity check must be run beforehand
 */
el str_to_el(char *input)
{
  double val;
  char *pos_str = malloc(BUFFER_OUT_POS * sizeof(char));
  sscanf(input, "%s=%lf", pos_str, &val);
  return init_el(str_to_pos(pos_str), val);
}
