CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lGL -lGLU -lGLEW -lSDL
SOURCES=main.cpp SDLApp.cpp Workshop2.cpp geometry.cpp ShaderProgram.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Workshop2

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf *o $(EXECUTABLE)
