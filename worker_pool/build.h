#!/bin/bash 
#gcc -shared -fPIC -std=gnu99 worker_pool.c -o libworker_pool.so -pthread
gcc worker_pool_test.c -o worker_pool_test -L./ -lworker_pool -std=gnu99

