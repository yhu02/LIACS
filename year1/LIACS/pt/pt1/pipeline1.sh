#!/bin/sh
bzgrep Personenauto Open_Data_RDW__Gekentekende_voertuigen-filtered.csv.bz2 | 
awk -F ',' '{print $1}' | 		#Get license plate number
tr [A-Z] X | 				#Change all letters to X
tr [0-9] 9 |				#Change all numbers to 9
sort | 
uniq
