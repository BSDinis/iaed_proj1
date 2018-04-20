/*
 *
 * sparse.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * defines the sparse datatype that represents a sparse matrix
 * defines several operations on that datatype
 *
 * dependencies:
 *   pos
 *   el
 */

/* Note to self: there may be some issues with your mem management
 * In particular your liberal usage of assignments and not cleaning up 
 * after yourself when using auxiliar sparse matrices */  

#include "sparse.h"

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

/* creates new instance of a sparse matrix */
static sparse init_new_sparse();

/* dependency of file_to_sparse
 * gets the first three values from the *.sm file */
static int get_sparse_values(FILE *stream, sparse *m);

/* dependency of file_to_sparse
 * gets the of elements from the *.sm file */
static int get_el_list(FILE *stream, sparse *m);

/* removes element from a position in the list */
static void remove_at(sparse *m, unsigned index);

/* updates max and min */
static void update_max_min(sparse *m);

/* removes elements from several positions */
static void remove_mult(sparse *m, unsigned index[], unsigned len);


/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/
/* 
 * initializes a matrix
 * may take a filename as input, in which case tries to load 
 * matrix of a *.sm file; if it fails, initializes a new matrix
 */
sparse init_sparse(int n, ...)
{
  sparse m;
  va_list valist;
  va_start(valist, n);

  /* if there is one argument, a valid file, load the matrix */
  if (n == 1) {
    char *filename = va_arg(valist, char *);
    va_end(valist);
    file_to_sparse(filename, &m);
    return m;
  }

  /* in any other case, create a new instance of the matrix */
  va_end(valist);
  return init_new_sparse();
}

/* 
 * creates new instance of a sparse matrix 
 *
 * the list starts with 100 elements, growing by a factor of 10
 * each time it is necessary, until it reaches the maximum size
 *
 * by omission, the zero is 0 and the min and max position are 
 * the same: (0, 0).
 */
static sparse init_new_sparse()
{
  sparse m;

  allocd(m) = 100;
  list(m) = malloc(allocd(m) * sizeof(el));
  zero(m) = 0;
  nelem(m) = 0; 
  min(m) = init_pos(0, 0);
  max(m) = init_pos(0, 0);

  return m;
}


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
  sparse aux;
  
  if (!valid_sm_file(filename)) {
    *m = init_new_sparse();
    return 1;
  }

  fp = fopen(filename, "r");

  /* gets the allocd, nelem and zero values,
   * allocates memory for the list,
   * initializes the max and min */
  if (get_sparse_values(fp, m) == 1) {
    *m = init_new_sparse();
    fclose(fp);
    return 1;
  }
    
  /* if the file has no elements, return */
  aux = *m;
  if (nelem(aux) == 0) {
    fclose(fp);
    return 0;
  }
  /* otherwise, get the list of values */
  else {
    get_el_list(fp, m);
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
  sparse aux = *m;

  if (fgets(input, BUFFER_OUT_EL, stream) == NULL) {
    free(input);
    return 1;
  }
  sscanf(input, "%d", &allocd(aux));

  if (fgets(input, BUFFER_OUT_EL, stream) == NULL) {
    free(input);
    return 1;
  }
  sscanf(input, "%d", &nelem(aux));

  if (!(allocd(aux) >= 100 && allocd(aux) <= MAX_N_ELEM && allocd(aux) % 10 == 0)
      || nelem(aux) > allocd(aux)) {
    free(input);
    return 1;
  }

  if (fgets(input, BUFFER_OUT_EL, stream) == NULL) {
    free(input);
    free_sparse(aux);
    return 1;
  }
  sscanf(input, "%lf", &zero(aux));


  list(aux) = malloc(allocd(aux) * sizeof(el));

  /* if the file has no elements, initialize an empty matrix */
  if (nelem(aux) == 0) {
    min(aux) = init_pos(0, 0);
    max(aux) = init_pos(0, 0);
  }

  *m = aux;
  free(input);
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
static int get_el_list(FILE *stream, sparse *m)
{
  int i;
  char *input = malloc(BUFFER_OUT_EL * sizeof(char));
  sparse aux = *m;

  /* sanity checks */
  if (fgets(input, BUFFER_OUT_EL, stream) == NULL || !valid_el(input)) {
    free(input);
    return 1;
  }                  

  /* the first element initializes the max and min positions
   * of the matrix, which are updated by every new input */
  list(aux)[0] = str_to_el(input);
  min(aux) = pos(list(aux)[0]);
  max(aux) = pos(list(aux)[0]);

  for (i = 1; i < nelem(aux); i++) {
    if (fgets(input, BUFFER_OUT_EL, stream) == NULL || !valid_el(input)) {
      free(input);
      return 1;
    }                  

    list(aux)[i] = str_to_el(input);
    max(aux) = max_pos(max(aux), pos(list(aux)[i]));
    min(aux) = min_pos(min(aux), pos(list(aux)[i]));
  }

  *m = aux;
  free(input);
  return 0;
}


/* destructor */
void free_sparse(sparse m)
{
  free(list(m));
}

/* exporting */

/*
 * converts a matrix into a file
 *
 * checks if the filename is valid.
 * if it isn't make it so, by adding the *.sm extension
 *
 * saves the matrix to file as it will be read:
 * the first three lines are the allocd, nelem and zero values
 * the proceding nelem lines are the elements
 *
 */
void sparse_to_file(sparse m, char *filename)
{
  FILE *fp;
  int i;
  int len = strlen(filename);

  if (!valid_sm_file(filename)) {
    len += 3;
    filename = (char *) realloc(filename, (len + 3) * sizeof(char));
    filename = strcat(filename, ".sm");
  }

  fp = fopen(filename, "w");
  
  fprintf(fp, "%u\n", allocd(m));
  fprintf(fp, "%u\n", nelem(m));
  fprintf(fp, "%lf\n", zero(m));

  for (i = 0; i < nelem(m); i++) {
    fprintf(fp, "%s\n", out_el(list(m)[i]));
  }
}



/*
 * adds a new element to the matrix
 * if the value is 'zero', removes the value in that position
 * if the position has a value, replace it.
 * may have to reallocate memory for the list
 * 
 * Return codes:
 *   0: Successful operation
 *   1: Space limit reached
 */
int add_el(sparse *m, el e)
{
  sparse aux = *m;
  int i;

  /* check if there is an equivalent position in the matrix
   *
   * if there is, replace or remove and exit.
   * important property: there aren't repeated positions
   */

  /* quick check */
  if (row(pos(e)) < row(min(aux)) && row(pos(e)) > row(max(aux)) &&
        col(pos(e)) < row(min(aux)) && col(pos(e)) > col(max(aux))) {

    for (i = 0; i < nelem(aux) && !eq_pos(pos(e), pos(list(aux)[i])); i++);

    if (i != nelem(aux)) {
      if(val(e) == zero(aux)) {
        remove_at(m, i);
        return 0;
      }
      else {
        val(list(aux)[i]) = val(e);
        *m = aux;
        return 0;
      }
    }
  }
  
  if (val(e) != zero(aux)) {
    if (nelem(aux) == allocd(aux)) {
      if (allocd(aux) < MAX_N_ELEM) {
        /* get more space and append the element */
        allocd(aux) *= 10;
        list(aux) = (el *) realloc(list(aux), allocd(aux) * sizeof(el));
        max(aux) = max_pos(max(aux), pos(e));
        min(aux) = min_pos(min(aux), pos(e));
        list(aux)[nelem(aux)++] = e;
      }
      else {
        /* space limit exceeded */
        return 1;
      }
    }
  }

  *m = aux;
  return 0;
}


/*
 * removes element from a position in the list
 *
 * note: doesn't make any verifications on what was on the position
 */
static void remove_at(sparse *m, unsigned index)
{
  int i;
  sparse aux = *m;

  /* if the position is greater than the number of elements, return
   * alternatively, if nelem(m) == 0, then the expression is true,
   * which deals with the empty matrix case */
  if (index >= nelem(aux)) return;

  nelem(aux)--;
  for (i = index; i < nelem(aux); i++) {
    list(aux)[i] = list(aux)[i + 1];
  }

  *m = aux;
  update_max_min(m);
}

/*
 * updates max and min
 *
 * may be necessary, since when removing there is no way of knowing
 * if the element was the one defining the max or min
 */
static void update_max_min(sparse *m)
{
  int i;
  sparse aux = *m;
  pos nmax, nmin;

  if (empty_sparse(aux)) {
    max(aux) = min(aux) = init_pos(0, 0);
    *m = aux;
    return;
  }

  nmax = nmin = pos(list(aux)[0]);
  for (i = 0; i < nelem(aux); i++) {
    nmax = max_pos(nmax, pos(list(aux)[i]));
    nmin = min_pos(nmin, pos(list(aux)[i]));
  }
  
  max(aux) = nmax;
  min(aux) = nmin;

  *m = aux;
}


/*
 * removes elements from several positions in the
 * ordered list index[]
 */
static void remove_mult(sparse *m, unsigned index[], unsigned len)
{
  if (len == 0) return;

  sparse aux = *m;
  el *new_list = malloc(allocd(aux) * sizeof(el));
  pos nmin = init_pos(UINT_MAX, UINT_MAX), nmax = init_pos(0, 0);
  int i, j, k;

  for (i = j = k = 0; i < nelem(aux) && k < len; i++) {
    if (i != index[k]) {
      new_list[j] = list(aux)[i];
      nmax = max_pos(nmax, pos(new_list[j]));
      nmin = min_pos(nmin, pos(new_list[j]));
      j++;
    }
    else {
      k++;
    }
  }

  list(aux) = new_list;
  max(aux) = nmax;
  min(aux) = nmin;
  nelem(aux) = j;

  *m = aux;
}


/*
 * prints all the elements on a sparse matrix
 * if the matrix is empty, print "empty matrix"
 */
void print_sparse(sparse m)
{
  if (empty_sparse(m)) {
    printf("empty matrix\n");
    return;
  }

  int i;
  for (i = 0; i < nelem(m); i++) {
    printf("%s\n", out_el(list(m)[i]));
  }
}


/*
 * prints the carachteristics of a sparse matrix
 */
void print_charact_sparse(sparse m)
{
  if (empty_sparse(m)) {
    printf("empty matrix\n");
    return;
  }

  /* ATTENTION: extra space! hackish solution: placeholder for char */
  printf("[%u %u] [%u %u] %u / %u = %lf%%\n",
      row(min(m)), col(min(m)), row(max(m)), col(max(m)),
      nelem(m), size_sparse(m), 100 * density_sparse(m));
}


/*
 * changes the value of zero
 * removes all occurrences of zero in the list
 */
void change_zero(sparse *m, double new_zero)
{
  sparse aux = *m;
  unsigned *indices;
  unsigned i, j;

  if (new_zero == zero(aux) || empty_sparse(aux)) return;

  zero(aux) = new_zero;
  indices = malloc(nelem(aux) * sizeof(int));
  for (i = j = 0; i < nelem(aux); i++) {
    if (val(list(aux)[i]) == zero(aux)) {
      indices[j++] = i;
    }
  }

  *m = aux;
  remove_mult(m, indices, j);
  free(indices);
}
