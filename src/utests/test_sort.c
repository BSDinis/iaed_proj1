/*
 * 
 * test_sort.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines interaction to test the element datatype
 */


/*
 * testing environment:
 *
 * receives a list of integers (one per line: the first is the size
 * of the array
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sort.h"

#define MAX_SIZE 10000

int normalize(int n, int low, int high);
unsigned key_int(item i);

int main()
{
  int size;
  int i;
  int m = 0, M = 9;
  fscanf(stdin, "%d", &size);
  size = (size > MAX_SIZE) ? MAX_SIZE : size;

  int vec[size];
  for (i = 0; i < size; i++) {
    fscanf(stdin, "%d", vec + i);
    vec[i] = normalize(vec[i], m, M);
    printf("%d:", vec[i]);
  }               
  printf("\n\n");

  counting_sort(vec, 0, size - 1, m, M, (*key_int));


  for (i = 0; i < size; printf("%d\n", vec[i++]));


  return 0;
}


int normalize(int n, int low, int high)
{
  n -= low;
  n %= (high + 1 - low);
  while (n < low)
    n += low;
  return n;
}

unsigned key_int(item i)
{
  return i;           
}

