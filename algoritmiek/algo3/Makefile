CC = g++

CompileParms = -c -Wall -std=c++11

OBJS = standaard.o beurs.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Beurs

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

beurs.o: beurs.cc standaard.h constantes.h beurs.h
	$(CC) $(CompileParms)  beurs.cc

main.o: main.cc standaard.h beurs.h
	$(CC) $(CompileParms)  main.cc

