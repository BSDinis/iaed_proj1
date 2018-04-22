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

#include "sparse.h"
#include "compress.h"
#include "sort.h"

/* define min and max functions as macros */
#define max_int(a, b) ((a > b) ? a : b)
#define min_int(a, b) ((a < b) ? a : b)

/* size of the buffer string to store the command */
#define CMD_BUFFER 100

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

/* compress a matrix */
void compress(sparse m, char *cmd);

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
        compress(m, cmd);
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
  unsigned i, j, k, row_i;
  el list[min_int(width_sparse(m), nelem(m))];
  sscanf(cmd + 2, "%u", &row_i);

  if (row_i < row(min(m)) || row_i > row(max(m))) {
    printf("empty line\n");
    return;
  }

  for (i = j = 0; i < nelem(m); i++) {
    if (row(pos(list(m)[i])) == row_i) {
      list[j++] = list(m)[i];
    }
  }
  if (j == 0) {
    printf("empty line\n");
    return;
  }
  j--;

  counting_sort(list, 0, j, col(min(m)), col(max(m)), &key_col);

  for (i = col(min(m)), k = 0; i <= col(max(m)); i++) {
    if (k <= j && col(pos(list[k])) == i) {
      printf(" %.3lf", val(list[k++]));
    }
    else {
      printf(" %.3lf", zero(m));
    }
  }
  printf("\n");
}


/* prints an entire column of the matrix */
void print_col(sparse m, char *cmd)
{
  unsigned i, j, k, col_i;
  el list[min_int(height_sparse(m), nelem(m))];
  char str[BUFFER_OUT_EL];
  sscanf(cmd + 2, "%u", &col_i);

  if (col_i < col(min(m)) || col_i > col(max(m))) {
    printf("empty column\n");
    return;
  }

  for (i = j = 0; i < nelem(m); i++) {
    if (col(pos(list(m)[i])) == col_i) {
      list[j++] = list(m)[i];
    }
  }
  if (j == 0) {
    printf("empty column\n");
    return;
  }
  j--;

  counting_sort(list, 0, j, row(min(m)), row(max(m)), &key_row);

  for (i = row(min(m)), k = 0; i <= row(max(m)); i++) {
    if (k <= j && row(pos(list[k])) == i) {
      out_el(list[k++], str);
    }
    else {
      out_el(init_el(0, init_pos(i, col_i)), str);
    }
    printf("%s\n", str);
  }
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
  
/* compress a matrix */
void compress(sparse m, char *cmd)
{
  //TODO;
  return;
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
