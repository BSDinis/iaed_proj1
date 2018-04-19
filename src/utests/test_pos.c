/*
 * 
 * test_pos.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines interaction to test the position datatype
 */


/*
 * testing environment:
 *
 * receives as input a sequence of strings, each one is a position
 * converts them to positions (tests valid_pos and str_to_pos)
 * finds the maximum and minimum position (tests max_pos and min_pos)
 * prints all columns (test selector)
 * prints all rows (test selector)
 * prints all positions (test out_pos)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pos.h"

int main()
{
  int size;
  int i, j;
  pos *list;
  pos max, min;
  char *input = malloc(BUFFER_OUT_POS * sizeof(int));

  fgets(input, BUFFER_OUT_POS, stdin);
  sscanf(input, "%d", &size);
  list = malloc(size * sizeof(pos));

  for (i = j = 0; i < size; i++) {
    if (fgets(input, BUFFER_OUT_POS, stdin) != NULL) {
      input[strlen(input) - 1] = 0;
      if (valid_pos(input)) {

        printf("%d: %s -> valid input\n", i + 1, input);
        list[j] = str_to_pos(input);

        if (j == 0) {
          max = min = list[i];
        }
        max = max_pos(max, list[j]);
        min = min_pos(min, list[j]);
        j++;
      }
      else {
        printf("%d: %s -> invalid input\n", i + 1, input);
      }
    }
  }
  printf("\n");

  for (i = 1; i < j; i++) {
    printf("%s %s %s\n", out_pos(list[i - 1]), (eq_pos(list[i - 1], list[i])) ? "==" : "!=", out_pos(list[i]));
  }
  printf("\n");
  for (i = 0; i < j; i++) printf("col: %u\n", col(list[i])); 
  printf("\n");
  for (i = 0; i < j; i++) printf("row: %u\n", row(list[i])); 
  printf("\n");
  for (i = 0; i < j; i++) printf("%s\n", out_pos(list[i])); 
  printf("\n");
  printf("\n");
  printf("max: %s\nmin: %s\n", out_pos(max), out_pos(min));


  return 0;
}



