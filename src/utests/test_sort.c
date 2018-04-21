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
unsigned key_1(item a);
unsigned key_2(item a);


int main()
{
  int size;
  int k;
  unsigned m[] = {4, 4};
  unsigned M[] = {16, 16};
  unsigned (*key_arr[])(item) = {key_1, key_2};

  fscanf(stdin, "%d", &size);
  size = (size > MAX_SIZE) ? MAX_SIZE : size;

  item vec[size];
  for (k = 0; k < size; k++) {
    fscanf(stdin, "%d %d", &(vec[k].x), &(vec[k].y));
    vec[k].x = normalize(vec[k].x, m[1], M[1]);
    vec[k].y = normalize(vec[k].y, m[0], M[0]);
    printf("(%d,%d):", vec[k].x, vec[k].y);
  }               
  printf("\n\n");

  radix_sort(vec, 0, size - 1, m, M, key_arr, 2);


  for (k = 0; k < size; printf("(%d,%d)\n", vec[k].x, vec[k].y), k++);


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

unsigned key_1(item a)
{
  return a.x;           
}

unsigned key_2(item a)
{
  return a.y;           
}

