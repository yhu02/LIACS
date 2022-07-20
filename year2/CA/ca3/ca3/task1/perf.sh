#!/bin/bash
make clean
make

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block1

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block2

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block4

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block8

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block16

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block32

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block64

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block128

perf stat -e instructions:u,cycles:u,cache-misses:u,cache-references:u ./matrixmul-r50-block256