#!/bin/sh
bzgrep Personenauto Open_Data_RDW__Gekentekende_voertuigen-filtered.csv.bz2 > temp1.csv #Decompress file and put into a temporary file for processing
grep Personenauto temp1.csv |
awk -F ',' '{print $3}' |				#Get brand
sort |
uniq -c |						#Count brands
awk -F ' ' '{if($1 > 100000)print $2}' | 		#Get brands with more than N = 100000 occurrences
awk -F ',' '	
	NR==FNR{					#Only apply to the first file i.e. stdin
		a[$1] = $1;				#Put all lines of first file in array a for pattern matching
		next;					#Skip rest of the code for faster processing 
	} 
	{for(i in a){					#Pattern match remaining files with first file
		if(a[i] == $3){			#Check if column 3 contains pattern denoted in first file
			print $2,$3,$21;		#Get type of car, brand and date
			}				
		}
	}
'  OFS=',' /dev/stdin temp1.csv |
grep Personenauto |					#Filter on type of car
awk -F ',' '{print $3 "," $2;}' |			#Get brand and date
sed 's/^......//' |					#Remove first 6 digits from date and get the year
awk -F ',' '{if($1 && $2){print $2 "," 2021-$1}}' |	#Check if date is greater than 0 and brand contains a string then print brand and 2021 - year
sort |
awk -F ',' '						#Calculate the average age
    {	
        arr[$1]   += $2;				#Add year of each line to array
        count[$1] += 1;				#Increase counter with 1 after each line
    }
    END{						#At the end, calculate average age and print brand and average age
        for (a in count) {
            print a "," (arr[a] / count[a]);
        }
    }'|
awk -F ',' '{printf "%0.f %s\n", $2,$1}' |		#Format float to round to the nearest number and reorder
sort -h
rm temp1.csv
