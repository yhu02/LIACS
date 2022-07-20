#!/bin/bash
make clean
make
make tilecomposite
rm task1_3.csv
for file in ../img/*
do {
    for file2 in ../img/*
    do {
        ./tilecomposite $file $file2 100 >> task1_3.csv
    }
    done
}
done