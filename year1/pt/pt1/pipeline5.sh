#!/bin/sh
#output 0 = saturday 1 = sunday etc. zeller's congruence
bzgrep Personenauto Open_Data_RDW__Gekentekende_voertuigen-filtered.csv.bz2 | 
awk -F ',' '{print $6}' |										#Get the date
awk '{
	century = substr($1,7,2);									#Split date into century, year, month and day
	year = substr($1,9,2);										
	month = substr($1,4,2);
	day = substr($1,1,2);
	if(month == "01" || month == "02"){								#Part of the algorithm, act as if January and February are month 13 and 14
		month = month + 1;
	}
	result = int(day + int((13/5)*(month+1))+year+int(year/4)+int(century/4)+5*century)%7; 	#Algorithm for calculating the day of the week
	if(result == 0){										#Print the day of the week based on result
		print "Saturday";
	}else if(result == 1){
		print "Sunday";
	}else if(result == 2){
		print "Monday";
	}else if(result == 3){
		print "Tuesday";
 	}else if(result == 4){
		print "Wednesday";
	}else if(result == 5){
		print "Thursday";
	}else if(result == 6){
		print "Friday";
	}
}' | 
sort |
uniq -c | 												#Get count for each day of the week
awk -F ' ' '							
{
	day[$2] = $1}											#Sort the days according to convention
	END{					
		print(day["Monday"] ",Monday\n" day["Tuesday"] ",Tuesday\n" day["Wednesday"] ",Wednesday\n" day["Thursday"] ",Thursday\n" day["Friday"] ",Friday\n" day["Saturday"] ",Saturday\n" day["Sunday"] ",Sunday\n")
}' | 
python -c '												#Python script for plotting				
import pandas
import matplotlib.pyplot as plot
import sys

df = pandas.read_csv(sys.stdin,sep=",",header=None,index_col=1)					#Read from stdin and use column 1(day of the week) as index
df.plot(kind="bar", rot = 30, legend=False)								#Plot configuration
plot.savefig("bar_plot.pdf")  
plot.show()
exit(0)										
'
