CC=g++
CFLAGS=-Wall -g `sdl-config --cflags`
SOURCES=main.cpp SDLApp.cpp Teapot.cpp geometry.cpp ShaderProgram.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
LDFLAGS=`sdl-config --libs` -lGLU -lGLEW  -lGL 

main: main.cpp
	$(CC) $(CPPFLAGS) -c $(SOURCES)
	$(CC) $(OBJECTS) $(CPPFLAGS) $(LDFLAGS) -o TeapotShader

clean:
	rm -rf $(OBJECTS) TeapotShader
