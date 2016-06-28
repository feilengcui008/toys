#!/bin/bash 
mpicc -o helloworld helloworld.c -std=c99
mpicc -o matrix_multiply ./matrix_multiply.c -std=c99
mpicc -o parallel_matrix_multiple ./parallel_matrix_multiple.c -std=c99
