CC = g++

CompileParms = -c -Wall -std=c++11

OBJS = standaard.o rooster.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Rooster

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

rooster.o: rooster.cc standaard.h constantes.h rooster.h
	$(CC) $(CompileParms)  rooster.cc

main.o: main.cc standaard.h rooster.h
	$(CC) $(CompileParms)  main.cc

