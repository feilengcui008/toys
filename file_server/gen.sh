#!/bin/bash 
cd ./ae 
./gen.sh 
cp libae.so ../
cd ..
gcc -o server server.c util.c -I./ae -L./ae -lae 
gcc -o client client.c util.c -I./ae -L./ae -lae 
