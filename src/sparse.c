/*
 *
 * sparse.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * main file
 * defines the sparse datatype that represents a sparse matrix
 * defines several operations on that datatype
 *
 * dependencies:
 *   pos
 *   el
 *   input
 *   compress
 *   sort
 */


#include <stdio.h>
#include <stdlib.h>

#include "pos.h"
#include "el.h"
#include "input.h"
#include "compress.h"
#include "sort.h"


#define MAX_N_ELEM 10000
#define MAX_NAME 80
#define INIT_SIZE 100

typedef struct {
  el *list;
  double zero;
  pos min, max;
  size_t nelem;
  size_t allocd;
} sparse;

int main(int argc, char *argv[])
{
  return 0;
}

/* 
 * creates a new instance of a sparse matrix
 * TODO: accept a filename as input, load the file into the matrix
 *
 */
sparse init_sparse()
{
  sparse m;

  m.nelem = 0;         
  m.allocd = INIT_SIZE;
  m.list = (el *) malloc(m.allocd * sizeof(el));
  m.zero = 0;
  m.min = m.max = {0, 0};
  return m;
}


/*
 * adds a new element to the matrix
 * if the value is 'zero', removes the value in that position
 * if the position has a value, replace it.
 * may have to reallocate memory for the list
 * 
 * Return codes:
 *   0: Successful addition
 *   1: Successful removal
 *   2: Successful substitution
 *   3: Space limit reached
 */
int add(sparse m, el new)
{
  /* find space to put the new element */
  if (m.nelem == m.allocd) {
    m.allocd *= 10;
    if (m.allocd > MAX_N_ELEM) {
      return 3;
    }

    m.list = (el *) realloc(m.list, m.allocd, sizeof(el));
  }

  for (int i = 0; i < m.nelem; i++) {
    if (eq_pos(m.list[i], new)) {
      if (new.val == m.zero) {
        /* remove element from list */
        m.nelem--;
        for (int j = i; j < m.nelem; j++) {
          m.list[j] = m.list[j + 1];
        }
        return 1;
      }
      else {
        /* change value */
        m.list[i].val = new.val;
        return 2;
      }
    }
  }
  m.list[m.nelem++] = new;
  return 0;
}


