#!/bin/bash
make clean
make tilecomposite
rm task1_3.csv
for file in ../img/*
do {
    for file2 in ../img/*
    do {
        valgrind --tool=cachegrind ./tilecomposite $file $file2 1
    }
    done
}
done