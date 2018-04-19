/*
 * 
 * test_el.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines interaction to test the element datatype
 */


/*
 * testing environment:
 *
 * receives as input a sequence of strings, each one is an element
 * converts them to elements (tests valid_el and str_to_el)
 * prints all values (test selector)
 * prints all elements (test out_el)
 * prints the comparison between elements (test comparison)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../el.h"

int main()
{
  int size;
  int i, j;
  el *list;
  char *input = malloc(BUFFER_OUT_EL * sizeof(int));
   
  fgets(input, BUFFER_OUT_EL, stdin);
  sscanf(input, "%d", &size);
  list = malloc(size * sizeof(el));


  for (i = j = 0; i < size; i++) {
    if (fgets(input, BUFFER_OUT_EL, stdin) != NULL) {
      input[strlen(input) - 1] = 0;
      if (valid_el(input)) {

        printf("%d: %s -> valid input\n", i + 1, input);
        list[j] = str_to_el(input);

        j++;
      }
      else {
        printf("%d: %s -> invalid input\n", i + 1, input);
      }
    }
  }

  printf("\n");
  for (i = 1; i < j; i++) {
    printf("%s %s %s; ", out_el(list[i - 1]), (less_el(list[i - 1], list[i])) ? "<" : ">=", out_el(list[i]));
    printf("%s %s %s\n", out_el(list[i - 1]), (eq_el(list[i - 1], list[i])) ? "==" : "!=", out_el(list[i]));
  }
  printf("\n");
  for (i = 0; i < j; i++) printf("val: %lf\n", val(list[i])); 
  printf("\n");
  for (i = 0; i < j; i++) printf("%s\n", out_el(list[i])); 

  return 0;
}



