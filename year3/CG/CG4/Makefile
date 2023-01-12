CC=g++
CFLAGS=-Wall -g `sdl-config --cflags`
SOURCES=main.cpp SDLApp.cpp workshop4.cpp geometry.cpp ShaderProgram.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
LDFLAGS=`sdl-config --libs` -lGLU -lGLEW  -lGL 

main: main.cpp
	$(CC) $(CPPFLAGS) -c $(SOURCES)
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o mountain

clean:
	rm -rf $(OBJECTS) mountain
