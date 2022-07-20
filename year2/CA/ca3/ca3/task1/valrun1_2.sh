#!/bin/bash
make clean
make grayscale
rm task1_2.csv
for file in ../img/*
do 
  valgrind --tool=cachegrind ./grayscale $file 1
done