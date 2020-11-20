C = g++
FLAGS = -Wall -Wextra -g

# ****************************************************
# Targets needed to bring the executable up to date

PM4: PM4.o grootGetal.o
	$(C) $(FLAGS) -o PM4 PM4.o grootGetal.o

# The main.o target can be written more simply
#
PM4.o: grootGetal.h stack.h

grootGetal.o: grootGetal.h stack.h