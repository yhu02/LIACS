CC = g++

CompileParms = -c -Wall -std=c++11

OBJS = standaard.o rooster.o main.o vak.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Rooster

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

vak.o: vak.cc vak.h
	$(CC) $(CompileParms)  vak.cc 

rooster.o: rooster.cc standaard.h constantes.h rooster.h vak.h
	$(CC) $(CompileParms)  rooster.cc

main.o: main.cc standaard.h rooster.h
	$(CC) $(CompileParms)  main.cc

