/*
 *
 * compress.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * header file for compress.c
 *
 * defines the compression capability
 *
 */

#ifndef COMPRESS_H
#define COMPRESS_H

#include "sparse.h"

/* compresses a sparse matrix, printing the output */
void compress_sparse(sparse m);

#endif /* !COMPRESS_H */
