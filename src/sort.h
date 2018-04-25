/*
 *
 * sort.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * header file for sort.c
 *
 * defines sorting functions
 *
 */

#ifndef SORT_H
#define SORT_H

#include "sparse.h"

/* sorts a sparse matrix, with regard to either the columns or the rows,
 * depending on a flag */
void sort_sparse(sparse *m, bool col);


#endif /* !SORT_H */
