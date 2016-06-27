#!/bin/bash
gcc -o libae.so -shared -fPIC ae.c anet.c atomicvar.h config.h fmacros.h zmalloc.c
