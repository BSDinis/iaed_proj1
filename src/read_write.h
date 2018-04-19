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

#include <stdbool.h>

/* TODO */
/* checks if the filename has a *.sm extension */
bool valid_sm_file(char *filename);

/* TODO */
/* converts a file to a list of strings */
int file_to_sparse(char *filename, char ***input);
