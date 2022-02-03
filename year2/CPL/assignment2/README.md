Class 2, Group 4
D.V.	Stokkermans	s2805197
S.S   Zeidi       s2982048
Y.	   Hu		      s2962802

The program works according to the requirements.

There are no know deviations from the assignment.

The program works by reading in a string from an input file given as a
command-line argument. We use regex to check for printable characters while
reading in the string from the file. This string then gets turned into a slice 
of tokens. In this process we check for illegal characters. After this the slice
of tokens is send to the parser. The parser builds a parse tree using recursive
descent. If there are no grammatical errors the process is succesfull. The tree
we have created is used to perform alpha conversions and beta reductions on.
This is done until no more actions is possible and leaves us with the most
reduced tree possible. The beta reduction is always performed on the highest 
lambda in the tree.
