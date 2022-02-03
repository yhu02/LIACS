CC = g++

CompileParms = -c -Wall -std=c++11

OBJS = standaard.o zet.o aapjeomino.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o AapjeOmino

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

zet.o: zet.cc standaard.h zet.h
	$(CC) $(CompileParms)  zet.cc

aapjeomino.o: aapjeomino.cc standaard.h zet.h aapjeomino.h
	$(CC) $(CompileParms)  aapjeomino.cc

main.o: main.cc standaard.h zet.h aapjeomino.h
	$(CC) $(CompileParms)  main.cc

