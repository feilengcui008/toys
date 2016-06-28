#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// multiply
int main(int argc, char **argv)
{
  int row_size1 = 4;
  int col_size1 = 8;
  int row_size2 = 8;
  int col_size2 = 4;
  if (argc >= 5) {
    row_size1 = atoi(argv[1]);
    col_size1 = atoi(argv[2]);
    row_size2 = atoi(argv[3]);
    col_size2 = atoi(argv[4]);
  }
  if (col_size1 != row_size2) {
    fprintf(stdout, "invalid format \n");
    return 0;
  }
  int *A, *B, *C;
  A = (int *)malloc(sizeof(int) * row_size1 * col_size1);
  B = (int *)malloc(sizeof(int) * row_size2 * col_size2);
  // result matrix
  C = (int *)malloc(sizeof(int) * row_size1 * col_size2);
  matrix(A, row_size1, col_size1);
  matrix(B, row_size2, col_size2);
  int tmp;
  for (int i = 0; i < row_size1; i++) {
    for (int j = 0; j < col_size2; j++) {
      tmp = 0;
      for (int n = 0; n < col_size1; n++) {
        tmp += A[i * col_size1 + n] * B[n * col_size2 + j];
      }
      C[i * col_size2 + j] = tmp;
    }
  }
  
  fprintf(stdout, "===== matrix A : =====\n");
  printMatrix(A, row_size1, col_size1);
  fprintf(stdout, "===== matrix B : =====\n");
  printMatrix(B, row_size2, col_size2);
  fprintf(stdout, "===== matrix C = AxB : =====\n");
  printMatrix(C, row_size1, col_size2);
  return 0;
}
