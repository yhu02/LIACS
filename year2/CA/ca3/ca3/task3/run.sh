#!/bin/bash
make clean
make

SECONDS=0
echo -n "0," > test.csv
./grayscale	../img/bricks512.png 10000
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed." >> test.csv
echo -e "\n" >> test.csv

SECONDS=0
echo -n "2," >> test.csv
./grayscale_2	../img/bricks512.png 10000
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed." >> test.csv
echo -e "\n" >> test.csv

SECONDS=0
echo -n "4," >> test.csv
./grayscale_4	../img/bricks512.png 10000
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed." >> test.csv
echo -e "\n" >> test.csv

SECONDS=0
echo -n "6," >> test.csv
./grayscale_6	../img/bricks512.png 10000
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed." >> test.csv
echo -e "\n" >> test.csv

SECONDS=0
echo -n "8," >> test.csv
./grayscale_8	../img/bricks512.png 10000
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed." >> test.csv
echo -e "\n" >> test.csv