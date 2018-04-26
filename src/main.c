/*
 *
 * main.c
 *
 * Baltasar Dinis 89416
 * IAED project
 *
 * main file
 *
 * defines the interaction for the program
 *
 * dependencies:
 *   sparse
 *   compress
 *   sort
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "sparse.h"
#include "compress.h"
#include "sort.h"

/* 
 * size of the buffer string to store the command 
 * worst case scenario is to add an element, which needs less than 350 chars
 * alloc 512 for good measure 
 */
#define CMD_BUFFER 512

/*
 * default filename, in case the user doesn't input one
 */
#define DEFAULT_FILENAME ".default_sparse.sm"

bool get_command(char cmd[CMD_BUFFER + 1]);

/*
 * dummy functions: 
 * parse the command and execute the appropriated functions
 */

void add(sparse *m, char cmd[CMD_BUFFER + 1]);

void print_row(sparse m, char cmd[CMD_BUFFER + 1]);

void print_col(sparse m, char cmd[CMD_BUFFER + 1]);

void sort(sparse *m, char cmd[CMD_BUFFER + 1]);

void ch_zero(sparse *m, char cmd[CMD_BUFFER + 1]);

void write_sparse(sparse m, char cmd[CMD_BUFFER + 1], char filename[MAX_FILENAME + 1]);

int main(int argc, char *argv[])
{
  sparse m;
  char cmd[CMD_BUFFER + 1];
  char filename[MAX_FILENAME + 1];

  /* ensure the filename starts as an empty string, 
   * in case of not being given as an argument */
  filename[0] = '\0';

  /* only allows two arguments */
  if (argc == 2) {
    strcpy(filename, argv[1]);
    m = init_sparse(1, filename);
  }
  else if (argc == 1) {
    m = init_sparse(0);
  }
  else {
    return 1;
  }

  while (get_command(cmd)) {
    switch (cmd[0]) {
      case 'a':
        add(&m, cmd);
        break;

      case 'p':
        print_sparse(m);
        break;

      case 'i':
        print_charact_sparse(m);
        break;

      case 'l':
        print_row(m, cmd);
        break;

      case 'c':
        print_col(m, cmd);
        break;

      case 'o':
        sort(&m, cmd);
        break;

      case 'z':
        ch_zero(&m, cmd);
        break;

      case 's':
        compress_sparse(m);
        break;

      case 'w':
        write_sparse(m, cmd, filename);
        break;

      default:
        break;
    }
  }

  return 0;
}


/* 
 * input: command
 *
 * removes all whitespace and newlines at the end of the command
 *
 * returns false if the command is quit or the stream reached EOF
 */
bool get_command(char cmd[CMD_BUFFER + 1])
{
  int i;
  if (fgets(cmd, CMD_BUFFER, stdin) != NULL) {

    /* remove trailing whitespace */
    i = strlen(cmd);
    while (isspace(cmd[--i]));
    cmd[i + 1] = '\0';

    return (strcmp(cmd, "q") != 0);
  }
  return false;
}


/* 
 * inputs: ptr to sparse, command
 *
 * adds a new element to the matrix 
 */
void add(sparse *m, char cmd[CMD_BUFFER + 1])
{
  /* note: "a <row> <col> <val>" ::
   * the element encoding starts in the 2nd position
   * this is common to all commands with arguments
   */
  add_el(m, str_to_el(cmd + 2));
}


/* 
 * inputs: sparse matrix, command
 * prints an entire row of the matrix 
 */
void print_row(sparse m, char cmd[CMD_BUFFER + 1])
{
  unsigned long i;
  sscanf(cmd + 2, "%lu", &i);
  print_row_i(m, i);
}

/* 
 * inputs: sparse matrix, command
 * prints an entire row of the matrix 
 */
void print_col(sparse m, char cmd[CMD_BUFFER + 1])
{
  unsigned long j;
  sscanf(cmd + 2, "%lu", &j);
  print_col_j(m, j);
}

/* 
 * inputs: ptr to sparse matrix, command
 * 
 * sorts a matrix:
 *   by rows, if the command is 'o'
 *   by columns, if the command is 'o column'
 */
void sort(sparse *m, char cmd[CMD_BUFFER + 1])
{
  if (strcmp(cmd, "o") == 0) {
    /* sort by rows */
    sort_sparse(m, false);
  }
  else if (strcmp(cmd, "o column") == 0) {
    /* sort by columns */
    sort_sparse(m, true);
  }
}
  
/* 
 * inputs: ptr to sparse matrix, command
 * changes the value of zero of a matrix 
 */
void ch_zero(sparse *m, char cmd[CMD_BUFFER + 1])
{
  double z;
  sscanf(cmd + 2, "%lf", &z);
  change_zero(m, z);
}

/* 
 * inputs: sparse matrix, command, and filename
 *
 * saves a matrix to a file 
 * has a default filename: DEFAULT_FILENAME 
 */
void write_sparse(sparse m, char cmd[CMD_BUFFER + 1], char filename[MAX_FILENAME + 1])
{
  if (strcmp(cmd, "w") != 0) {
    filename = strcpy(filename, cmd + 2);
  }

  if (strlen(filename) == 0) {
    filename = DEFAULT_FILENAME;
  }

  sparse_to_file(m, filename);
}
