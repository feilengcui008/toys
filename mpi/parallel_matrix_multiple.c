#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <unistd.h>

#include "util.h"

int main(int argc, char *argv[]) {
    int row_size1 = 8;
    int col_size1 = 10;
    int row_size2 = 10;
    int col_size2 = 5;
    if (argc >= 5) {
        row_size1 = atoi(argv[1]);
        col_size1 = atoi(argv[2]);
        row_size2 = atoi(argv[3]);
        col_size2 = atoi(argv[4]);
    }
    if (col_size1 != row_size2) {
        fprintf(stderr, "invalid format\n");
        return 0;
    }
    int myrank, nprocs;
    MPI_Status status;
    int i, j, n;
    int tmp = 0;
    int *A, *B, *C, *T, *buf, *ans;
    // init mpi
    MPI_Init(&argc, &argv);
    // get my signature
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    // get core number
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    // printf("process : rank %d of %d\n", myrank, nprocs);

    // divide matrix A into n pieces
    int block_row_size = row_size1 / nprocs;
    int block_col_size = col_size1;

    T = (int *)malloc(sizeof(int) * row_size2 * col_size2);
    buf = (int *)malloc(sizeof(int) * block_row_size * block_col_size);
    ans = (int *)malloc(sizeof(int) * block_row_size * col_size2);
    // main process
    if (myrank == 0) {
        printf("total processes : %d\n", nprocs);
        fprintf(stdout, "divide matrix A to %d blocks\n", nprocs);
        A = (int *)malloc(sizeof(int) * row_size1 * col_size1);
        B = (int *)malloc(sizeof(int) * row_size2 * col_size2);
        C = (int *)malloc(sizeof(int) * row_size1 * col_size2);

        matrix(A, row_size1, col_size1);
        matrix(B, row_size2, col_size2);

        fprintf(stdout, "\n=========== matrix A ===========\n");
        printMatrix(A, row_size1, col_size1);
        fprintf(stdout, "\n=========== matrix B ===========\n");
        printMatrix(B, row_size2, col_size2);

        for (i = 0; i < row_size1; i++) {
            for (j = 0; j < col_size2; j++) {
                tmp = 0;
                for (n = 0; n < col_size1; n++) {
                    tmp += A[i * col_size1 + n] * B[n * col_size2 + j];
                }
                C[i * col_size2 + j] = tmp;
            }
        }
        fprintf(stdout, "\n===== matrix C = A x B not computed by mpi =====\n");
        printMatrix(C, row_size1, col_size2);

        // send second matrix to other processes
        for (i = 1; i < nprocs; i++) {
            MPI_Send(B, row_size2 * col_size2, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // send seperate block to each other process
        for (n = 1; n < nprocs; n++) {
            MPI_Send(A + n * block_col_size * block_row_size,
                     block_row_size * block_col_size, MPI_INT, n, 1,
                     MPI_COMM_WORLD);
        }
        // receive results from other processes
        for (n = 1; n < nprocs; n++) {
            MPI_Recv(C + n * block_row_size * col_size2,
                     block_row_size * col_size2, MPI_INT, n, 3, MPI_COMM_WORLD,
                     &status);
        }
        // calculate my part
        for (i = 0; i < block_row_size; ++i) {
            for (j = 0; j < col_size2; ++j) {
                tmp = 0;
                for (n = 0; n < block_col_size; ++n) {
                    tmp += A[i * block_col_size + n] * B[n * col_size2 + j];
                }
                C[i * col_size2 + j] = tmp;
            }
        }
        sleep(2);
        fprintf(
            stdout,
            "\n===== matrix C = A x B computed parallel by open-mpi =====\n");
        printMatrix(C, row_size1, col_size2);
    } else {
        MPI_Recv(T, row_size2 * col_size2, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
        MPI_Recv(buf, block_row_size * block_col_size, MPI_INT, 0, 1,
                 MPI_COMM_WORLD, &status);
        // matrix multiplication
        for (i = 0; i < block_row_size; i++) {
            for (j = 0; j < col_size2; j++) {
                tmp = 0;
                for (n = 0; n < block_col_size; n++) {
                    tmp += buf[i * block_col_size + n] * T[n * col_size2 + j];
                }
                ans[i * col_size2 + j] = tmp;
            }
        }
        // send results to the main process
        // fprintf(stdout, "~~~~~ matrix block computed by process %d\n",
        // myrank);
        // printMatrix(ans, block_row_size, col_size2);
        MPI_Send(ans, block_row_size * col_size2, MPI_INT, 0, 3,
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
