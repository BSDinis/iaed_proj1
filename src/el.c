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
  val(e) = val;
  pos(e) = p;
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
 * <pos>=%lf
 */
bool valid_el(char *str)
{
  int i = 0;
  int cnt = 0;

  while (str[i] != '=' && str[i] != '\0') i++;
  if (str[i] == '\0') return false;

  /* truncates string, to find if the substring to the left of the
   * equal sign is a position */
  str[i] = '\0';
  if (!valid_pos(str)) return false;

  /* repositions the equal sign, for out_el */
  str[i++] = '=';

  /* may be negative */
  if (str[i] == '-') i++;

  /* wait for decimal dot, if there is one */
  while(str[i] != '\0' && str[i] != '.') {
    if (!isdigit(str[i++])) return false;
    cnt++;
  }

  if (str[i++] == '\0') return (cnt != 0);

  while(str[i] != '\0') {
    if (!isdigit(str[i++])) return false;
    cnt++;
  }

  return (cnt != 0);
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

  int i = 0;
  while (input[i] != '=') {
    pos_str[i] = input[i];
    i++;
  }

  /* 
   * the double starts after the =, which is in the 
   * (i + 1) th position of the input string
   */
  sscanf(input + i + 1, "%lf", &val);
  return init_el(val, str_to_pos(pos_str));
}
