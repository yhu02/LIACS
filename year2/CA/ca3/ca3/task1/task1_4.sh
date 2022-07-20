#!/bin/bash
make clean
make pipeline1
rm task1_4.csv
for file in ../img/*
do {
    for file2 in ../img/*
    do {
        for i in {0..10}
        do {
            ./pipeline1 $file $file2 1 >> task1_3.csv
        }
        done
    }
    done
}
done