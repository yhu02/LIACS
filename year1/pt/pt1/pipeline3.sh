#!/bin/sh
bzgrep Personenauto Open_Data_RDW__Gekentekende_voertuigen-filtered.csv.bz2 | 
awk -F ',' '{print $3 , $4 , $56}' OFS=','|	#Get brand and model and taxi license indicator
grep Ja |    					#Get all taxi licensed lines
sort |						
uniq -c |					#Count amount of same brand and model
sort -nr |
head -10 |					#Get top 10 
awk -F ' ' '{$1 = ""; print $0}' OFS=' '|	#Remove count numbers
sed 's/^ *//g' |				#Remove leading whitespace
awk -F ',' '{print $1 ,$2}' OFS=','           #Get brand and model
