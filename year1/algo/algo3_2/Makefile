CC = g++

CompileParms = -c -Wall -std=c++11 -O2 -static -g

OBJS = standaard.o haven.o main.o

Opdr: $(OBJS)
	$(CC) $(OBJS) -o Haven

standaard.o: standaard.cc standaard.h
	$(CC) $(CompileParms)  standaard.cc

haven.o: haven.cc standaard.h constantes.h haven.h
	$(CC) $(CompileParms)  haven.cc

main.o: main.cc standaard.h constantes.h haven.h
	$(CC) $(CompileParms)  main.cc

