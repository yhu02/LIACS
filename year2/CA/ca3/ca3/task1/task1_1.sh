#!/bin/bash
make clean
make
echo  "blocksize,matrix_size,matrix_time,iterations,calculation_time"  >  task1_1.csv
echo -n "Without loop blocking ..."
echo -e "\n*****************"	
SECONDS=0
echo -n "0," >> task1_1.csv
./matrixmul-r50-block1	>> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
echo -e "\n"
echo -n "With loop blocking"
SECONDS=0
echo -e "\n"
echo -n "2," >> task1_1.csv
echo -e "\n"
./matrixmul-r50-block2	>> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"
echo -n "4," >> task1_1.csv
echo -e "\n"
./matrixmul-r50-block4	>> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"
echo -n "8," >> task1_1.csv
echo -e "\n"
./matrixmul-r50-block8>> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"
echo -n "16," >> task1_1.csv
echo -e "\n"
./matrixmul-r50-block16 >> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"
echo -n "32," >> task1_1.csv
echo -e "\n"
./matrixmul-r50-block32 >> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"
echo -n "64," >> task1_1.csv
./matrixmul-r50-block64 >> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
SECONDS=0
echo -e "\n*****************"	
echo -n "128," >> task1_1.csv
./matrixmul-r50-block128 >> task1_1.csv
duration=$SECONDS
echo -n "$(($duration / 60)) minutes and $(($duration % 60)) seconds elapsed."
echo -e "\n"
echo -n "FINISHED"
echo -e "\n"

	