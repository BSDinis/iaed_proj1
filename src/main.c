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

#include "sparse.h"
#include "compress.h"
#include "sort.h"

/* size of the buffer string to store the command 
 * worst case scenario is to add an element, which needs less than 350 chars
 * alloc 512 for good measure */
#define CMD_BUFFER 512

/* gets a string with a command 
 * returns false if the command is quit or the stream reached EOF */
bool get_command(char *cmd);

/* adds a new element to the matrix */
void add(sparse *m, char *cmd);

/* prints an entire row of the matrix */
void print_row(sparse m, char *cmd);

/* prints an entire column of the matrix */
void print_col(sparse m, char *cmd);

/* sorts a sparse matrix, with regard to either the cols or the rows */
void sort(sparse *m, char *cmd);

/* changes the value of zero of a matrix */
void ch_zero(sparse *m, char *cmd);

/* saves a matrix to a file */
void write_sparse(sparse m, char *cmd, char *filename);


int main(int argc, char *argv[])
{
  sparse m;
  char cmd[CMD_BUFFER + 1];
  char filename[MAX_FILENAME + 1];
  char command;

  /* ensure the filename starts as an empty string */
  filename[0] = '\0';

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
    command = cmd[0];
    switch (command) {
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
 * gets a string with a command 
 * returns false if the command is quit or the stream reached EOF
 */
bool get_command(char *cmd)
{
  if (fgets(cmd, CMD_BUFFER, stdin) != NULL) {
    /* remove newline character */
    cmd[strlen(cmd) - 1] = '\0';  
    return (strcmp(cmd, "q") != 0);
  }
  return false;
}


/* adds a new element to the matrix */
void add(sparse *m, char *cmd)
{
  /* note: "a <row> <col> <val>" :: the element encoding starts in the 2nd 
   * position */
  add_el(m, str_to_el(cmd + 2));
}


/* prints an entire row of the matrix */
void print_row(sparse m, char *cmd)
{
  unsigned i;
  sscanf(cmd + 2, "%u", &i);
  print_row_i(m, i);
}

/* prints an entire column of the matrix */
void print_col(sparse m, char *cmd)
{
  unsigned j;
  sscanf(cmd + 2, "%u", &j);
  print_col_j(m, j);
}

/* sorts a sparse matrix, with regard to either the cols or the rows */
void sort(sparse *m, char *cmd)
{
  if (strcmp(cmd, "o") == 0) {
    /* sort by rows */
    sort_sparse(m, false);
  }
  else if (strcmp(cmd, "o column") == 0) {
    /* sort by columns */
    sort_sparse(m, true);
  }
  /* quietly exits when finds a wrong command argument */
}
  
/* changes the value of zero of a matrix */
void ch_zero(sparse *m, char *cmd)
{
  double z;
  sscanf(cmd + 2, "%lf", &z);
  change_zero(m, z);
}

/* saves a matrix to a file */
void write_sparse(sparse m, char *cmd, char *filename)
{
  if (strcmp(cmd, "w") != 0) {
    strcpy(filename, cmd + 2);
  }

  if (strlen(filename) > 0) {
    sparse_to_file(m, filename);
  }
}
