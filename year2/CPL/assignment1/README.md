Class 2, Group 1
D.V.	Stokkermans	s2805197
J. 	    de Waart	s2
Y.	    Hu		    s2962802

The program works according to the requirements. However it does not get rid of
all redundant parentheses.

There are no know deviations from the assignment.

The program works by reading in a string from an input file given as a
command-line argument. We use regex to check for printable characters while
reading in the string from the file. This string then gets turned into a slice 
of tokens. In this process we check for illegal characters. After this the slice
of tokens is send to the parser. The parser builds a parse tree using recursive
descent and places parentheses where needed. If there are no grammatical errors
the process is succesfull and we get a string as output. This string is then
send back in for a second check. We expect to see the same result as after the
first time.