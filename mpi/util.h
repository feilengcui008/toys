#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// generate matrix randomly
void matrix(int* a, int row_size, int col_size)
{
  int i, j;
  srand(time(NULL));
  for (i = 0; i < row_size; i++) {
    for (j = 0; j < col_size; j++) {
      a[i * col_size + j] = (rand() % 10);
    }
  }
}

void printMatrix(int *a, int x, int y)
{
  for (int i = 0; i < x; ++i) {
    for (int j = 0; j < y; ++j) {
      fprintf(stdout, "%d\t", a[i * x + j]);
    }
    fprintf(stdout, "\n");
  }
}

#endif
