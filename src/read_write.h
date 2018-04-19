/*
 *
 * read_write.h
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * header file for read_write.c
 *
 * defines the input functions
 *
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sparse.h"

/* checks if the filename has a *.sm extension */
bool valid_sm_file(char *filename);

/* converts a file to a sparse matrix */
int file_to_sparse(char *filename, sparse *m);
