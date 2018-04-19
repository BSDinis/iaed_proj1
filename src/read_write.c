/*
 *
 * read_write.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines read_write functions
 */

#include "read_write.h"

/* dependency of file_to_sparse
 * gets the first three values from the *.sm file */
static int get_sparse_values(FILE *stream, sparse *m);

/* dependency of file_to_sparse
 * gets the of elements from the *.sm file */
static int get_list_values(FILE *stream, sparse *m);

/* checks if the filename has a *.sm extension */
bool valid_sm_file(char *filename)
{
  int i = strlen(filename);

  /* minimal filename: a.sm -> 4 chars */
  if (i < 4) return false;

  /* position i on the dot */
  i -= 3;
  
  /* the last three characters must be ".sm" */
  return strcmp(filename + i, ".sm");
}

/* 
 * converts a file to a sparse matrix
 * the coversion is atomic: 
 * the function either coverts the whole file or initializes
 * the matrix as an empty, returning the error function
 *
 * return codes:
 *   0: sucessful coversion
 *   1: unsucessful coversion
 *  
 */
int file_to_sparse(char *filename, sparse *m)
{
  FILE *fp;
  
  if (!valid_sm_file(filename)) {
    *m = init_sparse(0);
    return 1;
  }

  fp = fopen(filename, "r");

  /* gets the allocd, nelem and zero values,
   * allocates memory for the list,
   * initializes the max and min */
  if (get_sparse_values(fp, m) == 1) {
    *m = init_sparse(0);
    fclose(fp);
    return 1;
  }
    
  /* if the file has no elements, return */
  if (nelem(m) == 0) {
    fclose(fp);
    return 0;
  }
  /* otherwise, get the list of values */
  else {
    get_list_values(FILE *stream, sparse *m)
  }

  fclose(fp);
  return 0;
}

/*
 * dependency of file_to_sparse
 *
 * gets the first three values from the *.sm file
 * allocd, nelem, zero
 *
 * initializes the list
 * initializes the min and max
 *
 * return codes:
 *   0: all well, proceed
 *   1: abort file_to_sparse execution
 */
static int get_sparse_values(FILE *stream, sparse *m)
{
  char *input = malloc(BUFFER_OUT_EL * sizeof(char));

  if (fgets(input, BUFFER_EL_OUT, fp) == NULL) {
    free(input);
    return 1;
  }
  sscanf(input, "%d", &allocd(*m));

  if (fgets(input, BUFFER_EL_OUT, fp) == NULL) {
    free(input);
    return 1;
  }
  sscanf(input, "%d", &nelem(*m));

  if (!(allocd(*m) >= 100 && allocd(*m) <= MAX_N_ELEM && allocd(*m) % 10 == 0)
      || nelem(*m) > allocd(*m)) {
    free(input);
    return 1;
  }

  if (fgets(input, BUFFER_EL_OUT, fp) == NULL) {
    free(input);
    return 1;
  }
  sscanf(input, "%lf", &zero(*m));


  list(*m) = malloc(allocd(*m) * sizeof(el));

  /* if the file has no elements, initialize an empty matrix */
  if (nelem(*m) == 0) {
    min(*m) = init_pos(0, 0);
    max(*m) = init_pos(0, 0);
  }

  free(input)
  return 0;
}


/*
 * dependency of file_to_sparse
 *
 * gets the list of elements
 * updates continuously the max and min
 *
 * return codes:
 *   0: all well, proceed
 *   1: abort file_to_sparse execution
 */
static int get_list_values(FILE *stream, sparse *m)
{
  int i;
  char *input = malloc(BUFFER_OUT_EL * sizeof(char));

  /* sanity checks */
  if (fgets(input, BUFFER_EL_OUT, fp) == NULL || !valid_el(input)) {
    free(input);
    return 1;
  }                  

  /* the first element initializes the max and min positions
   * of the matrix, which are updated by every new input */
  list(*m)[0] = str_to_el(input);
  min(*m) = pos(list(*m)[0]);
  max(*m) = pos(list(*m)[0]);

  for (i = 1; i < nelem(*m); i++) {
    if (fgets(input, BUFFER_EL_OUT, fp) == NULL || !valid_el(input)) {
      free(input);
      return 1;
    }                  

    list(*m)[i] = str_to_el(input);
    max(*m) = max_pos(max(*m), pos(list(*m)[i]));
    min(*m) = min_pos(min(*m), pos(list(*m)[i]));
  }

  free(input);
  return 0;
}
