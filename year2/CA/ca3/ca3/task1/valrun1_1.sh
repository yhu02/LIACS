#!/bin/bash
make clean
make
echo -e "\n***************** no block"
valgrind --tool=cachegrind ./matrixmul-r50-block1
echo -e "\n***************** block 2"
valgrind --tool=cachegrind ./matrixmul-r50-block2
echo -e "\n***************** block 4"
valgrind --tool=cachegrind ./matrixmul-r50-block4
echo -e "\n***************** block 8"
valgrind --tool=cachegrind ./matrixmul-r50-block8
echo -e "\n***************** block 16"
valgrind --tool=cachegrind ./matrixmul-r50-block16
echo -e "\n***************** block 32"
valgrind --tool=cachegrind ./matrixmul-r50-block32
echo -e "\n***************** block 64"
valgrind --tool=cachegrind ./matrixmul-r50-block64
echo -e "\n***************** block 128"
valgrind --tool=cachegrind ./matrixmul-r50-block128
echo -e "\n***************** block 256"
valgrind --tool=cachegrind ./matrixmul-r50-block256
echo -e "\n***************** FINISHED"