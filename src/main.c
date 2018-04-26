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

/* note: the command arguments start on the 2nd position
 * eg: "a <row> <col> <val>" 
 * for that reason, reading from cmd + 2 is common
 */

/*-------------------------------*/
/* prototypes */
/*-------------------------------*/

bool get_command(char cmd[CMD_BUFFER + 1]);

void add(sparse *m, char cmd[CMD_BUFFER + 1]);

void print_row(sparse m, char cmd[CMD_BUFFER + 1]);

void print_col(sparse m, char cmd[CMD_BUFFER + 1]);

void sort(sparse *m, char cmd[CMD_BUFFER + 1]);

void ch_zero(sparse *m, char cmd[CMD_BUFFER + 1]);

void write_sparse(sparse m, char cmd[CMD_BUFFER + 1], char filename[MAX_FILENAME + 1]);

/*-------------------------------*/
/*-------------------------------*/
/*-------------------------------*/


int main(int argc, char *argv[])
{
  sparse m;
  char cmd[CMD_BUFFER + 1];
  char filename[MAX_FILENAME + 1] = {'\0'};
  char command;


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
 * input: buffer to be written
 * writes the command (with the whitespace at the end removed)
 */
bool get_command(char cmd[CMD_BUFFER + 1])
{
  int i;
  if (fgets(cmd, CMD_BUFFER, stdin) != NULL) {

    /* remove whitespace*/
    i = strlen(cmd);
    while (isspace(cmd[--i]));
    cmd[i + 1] = '\0';
    return (strcmp(cmd, "q") != 0);

  }

  return false;
}


/* 
 * input: ptr to sparse, command
 * 
 * adds a new element to the matrix
 */
void add(sparse *m, char cmd[CMD_BUFFER + 1])
{
  add_el(m, str_to_el(cmd + 2));
}


/* 
 * input: sparse matrix, command
 * 
 * prints an entire row of the matrix
 */
void print_row(sparse m, char cmd[CMD_BUFFER + 1])
{
  unsigned long i;
  sscanf(cmd + 2, "%lu", &i);
  print_row_i(m, i);
}

/* 
 * input: sparse matrix, command
 * 
 * prints an entire column of the matrix
 */
void print_col(sparse m, char cmd[CMD_BUFFER + 1])
{
  unsigned long j;
  sscanf(cmd + 2, "%lu", &j);
  print_col_j(m, j);
}

/*
 * input: ptr to sparse matrix, command
 *
 * sorts a matrix either by columns or rows
 */
void sort(sparse *m, char cmd[CMD_BUFFER + 1])
{
  if (strcmp(cmd, "o") == 0) {
    /* sort by rows */
    sort_sparse(m, false);
  }
  else {
    /* sort by columns */
    sort_sparse(m, true);
  }
}
  
/*
 * input: ptr to sparse matrix, command
 *
 * changes the zero value of a matrix
 */
void ch_zero(sparse *m, char cmd[CMD_BUFFER + 1])
{
  double z;
  sscanf(cmd + 2, "%lf", &z);
  change_zero(m, z);
}

/*
 * input: sparse matrix, command, filename
 *
 * saves the sparse to a file: filename
 */
void write_sparse(sparse m, char cmd[CMD_BUFFER + 1], char filename[MAX_FILENAME + 1])
{
  if (strcmp(cmd, "w") != 0) {
    filename = strcpy(filename, cmd + 2);
  }

  if (strlen(filename) > 0) {
    sparse_to_file(m, filename);
  }
}
