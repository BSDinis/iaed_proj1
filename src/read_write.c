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

/*
 * given a valid file
 *
 * return a list of strings, each representing an element of the sparse
 * matrix
 * the first two strings will be unsigned integers:
 *  the first is the allocated memory necessary to hold the string 
 *  (in conformity with the assumption that the initial allocation is for
 *  100 positions and that, until the limit is reached, that size is 
 *  multiplied by 100 when necessary)
 *
 *  the second is the number of elements
 *
 * the third will be a double, representing the zero of the matrix
 *
 * a file is valid if has a '.sp' extension in the end 
 * [and if the first line corresponds to a hash calculated using the 
 * information of the matrix| maybe later; TODO]
 */


/*
 * TODO:
 *  - implement the encoding to a file
 *  - implement the deconding from a file
 */


/* TODO */
/* checks if the filename has a *.sm extension */
bool valid_sm_file(char *filename)
{
  return true;
}

/* TODO */
/* converts a file to a list of strings */
int file_to_sparse(char *filename, char ***input)
{
  return 1;
}

