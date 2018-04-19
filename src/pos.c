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

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

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
  char *str = (char *) malloc(BUFFER_OUT_POS * sizeof(char));
  if (snprintf(str, BUFFER_OUT_POS, "[%u;%u]", row(p), col(p)) >= BUFFER_OUT_POS) {
    perror("pos.c: out_pos: buffer overflow trying to output pos.\n");
    printf("pos.c: out_pos: buffer overflow trying to output pos.\n");
    exit(1);
  }
  return str;
}


/* 
 * valid string representation of an element 
 *
 * checks if a string matches a representation of an position:
 * [%u;%u]
 */
bool valid_pos(char *str)
{
  int i = 0;
  int cnt = 0;
  if (str[i++] != '[') return false;

  while (str[i] != ';' && str[i] != '\0') {
    if (!isdigit(str[i++])) return false; 
    cnt++;
  }
  if (str[i] == '\0' || str[i] != ';' || cnt == 0) return false;
  i++;

  cnt = 0;
  while (str[i] != ']' && str[i] != '\0') {
    if (!isdigit(str[i++])) return false;
    cnt++;
  }
  if (str[i] == '\0' || str[i] != ']' || cnt == 0) return false;

  i++;
  return (str[i] == '\0');
}

/* 
 * input from string 
 * given a string of the format: [<row>;<col>]
 * returns the (<row>,<col>) position
 *
 * does not accomodate for invalid input strings, prior checking 
 * must be done
 */
pos str_to_pos(char *input)
{
  pos p;
  sscanf(input, "[%u;%u]", &row(p), &col(p));
  return p;
}

/* maximum and minimum */

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
