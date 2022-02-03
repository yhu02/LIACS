CC = g++ 

CFLAGS := -c -fPIC -Wall -Wextra -std=c++11 

LIBFLAGS := -fPIC -shared

LFLAGS := -L. -I. -lpt3

OBJS := stringseq.o main.o variant.o stringseqvariant.o arrayseq.h dlinkedlistseq.h arrayseqvariant.h dlinkedlistseqvariant.h testprogram.o standard.o

TARGET_LIB := libpt3.so

PROGRAM_NAME := pt3


all: $(TARGET_LIB) $(PROGRAM_NAME)
	$(CC) $(LFLAGS) -o $(PROGRAM_NAME)

$(TARGET_LIB): $(OBJS) 
	$(CC) $(LIBFLAGS) -o $(TARGET_LIB) $(OBJS)
	rm *.o

link:
	$(CC) $(LFLAGS) -o $(PROGRAM_NAME)

$(PROGRAM_NAME): main.cpp testprogram.h
	$(CC) $(CFLAGS)	main.cpp -o $(PROGRAM_NAME)

testprogram.o: testprogram.cpp testprogram.h
	$(CC) $(CFLAGS)	testprogram.cpp

standard.o: standard.cpp standard.h
	$(CC) $(CFLAGS)	standard.cpp

stringseq.o: stringseq.cpp stringseq.h 
	$(CC) $(CFLAGS)	stringseq.cpp	

variant.o: variant.cpp variant.h
	$(CC) $(CFLAGS)	variant.cpp

stringseqvariant.o: stringseqvariant.cpp stringseqvariant.h 
	$(CC) $(CFLAGS)	stringseqvariant.cpp  


clean:
	rm *.o *.so pt3 RESULT.txt TEMP.txt VARIANTS.txt
	