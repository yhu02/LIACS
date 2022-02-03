#!/bin/sh
bzgrep Personenauto Open_Data_RDW__Gekentekende_voertuigen-filtered.csv.bz2 > temp1.csv #Decompress file and put into a temporary file for processing
grep Personenauto temp1.csv | 
awk -F ',' '$24 ~ /[0-9\.]/ && $51 ~ /[0-9\,]/{print $3}'|  		#Get brand if price and mass are valid numbers
sort | 
uniq -c |								#Count brands
awk -F ' ' '{if($1 > 10000)print $2}' |				#Get brands with more than N=10000 occurrences
awk -F ',' '	
	NR==FNR{							#Only apply to the first file i.e. stdin
		a[$1] = $1;						#Put all lines of first file in array a for pattern matching
		next;							#Skip rest of the code for faster processing 
	} 
	{for(i in a){							#Pattern match remaining files with first file
		if(a[i] == $3){					#Check if column 3 contains pattern denoted in first file
			print $3, $24, $51;				#Get brand, price and mass
			}
		}
	}
'  OFS=',' /dev/stdin temp1.csv |
awk -F ',' '
$2 ~ /[0-9\.]/ && $3 ~ /[0-9\,]/{print $0}' OFS="," | 		#Filter on valid price and mass
sort | 
awk -F ',' '								#Calculate the average price and mass
    {						
        cataloguswaarde[$1]    += $2					#Add price of each line to array
        vermogen[$1]   	+= $3					#Add mass of each line to array
        count[$1] 		+= 1					#Increase counter with 1 after each line
    }
    END{								#At the end, calculate average price and mass, and print brand and average price and average mass
        for (a in count) {
            print a "," (cataloguswaarde[a] / count[a]) "," (vermogen[a] / count[a])
        }
    }' | 
python -c '								#Python script for plotting
import matplotlib.pyplot as plot
import pandas
import sys

df = pandas.read_csv(sys.stdin,sep=",",header=None,index_col=0)	#Read from stdin and use column 1(day of the week) as index
df.columns=["Cataloguswaarde", "Vermogen"]				#Rename columns for ease of use
df.plot.scatter(x="Cataloguswaarde", y="Vermogen", legend=True)	#Plot configuration
plot.savefig("scatter_plot.pdf")  
plot.show()
exit(0)
'
rm temp1.csv
