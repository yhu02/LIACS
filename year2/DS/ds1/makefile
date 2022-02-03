CC = g++

CompileParms = -c -g -Wall

OBJS =  main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o main

main.o: main.cpp array.h linkedlist.h maxstapel.h stlstack.h stlvector.h
	$(CC) $(CompileParms)  main.cpp

